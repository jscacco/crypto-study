#*****************************************************************************
# Name: Jack Scacco
# Date: 9/13/19
# File: block.py
# Assignment: Project 2
# Purpose: Implement a block cipher, including various modes
#*****************************************************************************

import random
import re
import sys
import math
import os

# # # # # # # # # # # # # Generic Helper Functions # # # # # # # # # # # # # # 

def get_op():
    """Gets the operation to be computed from the command line"""
    if len(sys.argv) < 2:
        print("Error! You must enter either '-e' or '-d' as the first argument.")
        return -1
    return sys.argv[1]


def get_func(op):
    """Returns the function to be used based on op."""
    if op == "-e":
        return func
    elif op == "-d":
        return unfunc

    
def get_input_file():
    """Get the input file name from the command line"""
    if len(sys.argv) < 3:
        return -1
    return sys.argv[2]


def get_output_file():
    """Get the output file name from the command line"""
    if len(sys.argv) < 4:
        return -1
    return sys.argv[3]


def rand_val(max):
    """Returns a pseudorandom value < max"""
    order = math.ceil(math.log10(max)) #Determine the num of digits in size
    index = math.floor(random.SystemRandom().random() * (10 ** order))

    # Yea, this is quite inefficient
    while (index >= max):
        index = math.floor(random.SystemRandom().random() * (10 ** order))

    return index


def byte_str_to_int(str):
    """Takes a byte string and converts it to an int."""
    return int.from_bytes(str, byteorder = "big")

def int_to_byte_str(num, size):
    """Takes an int and converts it to a byte str."""
    return num.to_bytes(size, byteorder = "big")

# # # # # # # # # # # # # # # # # # Input # # # # # # # # # # # # # # # # # #


def read_block(file, block_size):
    """Reads in a block of size block_size from the input file. Returns False if EOF"""
    block = b""
    for i in range(block_size):
        this_byte = file.read(1)
        # If the last block consumed the last char in file:
        if this_byte == b"" and i == 0:
            return (-1, False)
        # If we reach EOF prematurely:
        elif this_byte == b"":
            block += chr(0).encode()*(block_size - i)
            return (block, False)
        else:
            block += this_byte
    return (block, True)


def is_empty(file):
    """Returns true if the file is empty"""
    with open(file, 'rb') as f:
        return not f.read(1)

    
# # # # # # # # # # # # # # # # # Function # # # # # # # # # # # # # # # # #

def xor(plaintext, key):
    """Takes two byte strings, xors them, and returns the resulting byte string"""
    # NOTE: this will return a string of length equal to the shorter of the two lengths
    
    # Iterate through the strings, creating a list of bytes
    arr = [chr(a ^ b) for (a,b) in zip(plaintext, key)]
    bstr = b"" # Initialize a byte string
    for byte in arr: # For each byte in the list,
        bstr += bytes([ord(byte)]) # Convert the byte in the list to a byte string
    return bstr

def func(plaintext, key):
    """Encrypts the plaintext using the key"""
    ciphertext = xor(plaintext, key)
    return ciphertext

def unfunc(ciphertext, key):
    """Decrypts the ciphertext using the key"""
    plaintext = xor(ciphertext, key)
    return plaintext


# # # # # # # # # # # # # # # # ECB Mode (1) # # # # # # # # # # # # # # # # #


def ecb(in_file, out_file, block_size, key, op):
    """Encrypts using func and the passed information under EBC mode."""

    my_func = get_func(op)
    
    with open(in_file, 'rb') as input: # Open files
        with open(out_file, 'wb') as output:
            block = [True, True] # This is just to get into the while loop
            while block[1]: # Iterate through the rest of the input
                block = read_block(input, block_size)
                if block[0] != -1: # Make sure there is at least one char in there
                    output.write(my_func(block[0], key))


# # # # # # # # # # # # # # # # CBC Mode (2) # # # # # # # # # # # # # # # # #


def cbc(in_file, out_file, block_size, key, op):
    """Encrypts using func and the passed information under CBC mode."""
    with open(in_file, 'rb') as input: # Open files
        with open(out_file, 'wb') as output:
            block = [True, True] # This is just to get into the while loop
            prev_block = read_block(input, block_size)[0] # This is the IV
            output.write(prev_block)
            
            while block[1]: # Iterate through the rest of the input
                block = read_block(input, block_size)
                if block[0] != -1: # Make sure there is at least one char in there
                    if (op == "-e"):
                        block_xor = xor(block[0], prev_block) 
                        block_cipher = func(block_xor, key)
                        output.write(block_cipher)
                        prev_block = block_cipher
                    elif (op == "-d"):
                        block_xor = unfunc(block[0], key)
                        block_message = xor(block_xor, prev_block)
                        output.write(block_message)
                        prev_block = block[0]


# # # # # # # # # # # # # # # # OFB Mode (3) # # # # # # # # # # # # # # # # #


def ofb(in_file, out_file, block_size, key, op):
    """Encrypts using func and the passed information under OFB mode."""
    
    with open(in_file, 'rb') as input: # Open files
        with open(out_file, 'wb') as output:
            block = [True, True] # This is just to get into the while loop
            prev_block = read_block(input, block_size)[0] # This is the IV
            output.write(prev_block)
            
            while block[1]: # Iterate through the rest of the input
                block = read_block(input, block_size)
                if block [0] != -1:
                    if (op == "-e"):
                        funced_block = func(prev_block, key)
                        prev_block = funced_block
                        block_xor = xor(block[0], funced_block) 
                        output.write(block_xor)
                    elif (op == "-d"):
                        funced_block = func(prev_block, key)
                        prev_block = funced_block
                        block_message = xor(block[0], funced_block)
                        output.write(block_message)


# # # # # # # # # # # # # # # # CTR Mode (4) # # # # # # # # # # # # # # # # #


def ctr(in_file, out_file, block_size, key, op):
    """Encrypts using func and the passed information under CTR mode."""
    
    with open(in_file, 'rb') as input: # Open files
        with open(out_file, 'wb') as output:
            ctr_str = read_block(input, block_size)[0] # This is the initial ctr
            ctr = byte_str_to_int(ctr_str)
            i = 0 # This is the value which will be added to ctr as we loop
            size = 2**(block_size * 8) # This is the length of the block size in bits
            
            output.write(int_to_byte_str(ctr + i, block_size))
            i += 1

            block = [True, True] # This is just to get into the while loop
            while block[1]: # Iterate through the rest of the input
                block = read_block(input, block_size)
                if block [0] != -1:
                    this_ctr = (ctr + i) % size
                    i += 1
                    funced_block = func(int_to_byte_str(this_ctr, block_size), key)
                    block_xor = xor(block[0], funced_block)
                    output.write(block_xor)

                    
# # # # # # # # # # # # # # # # # # Main # # # # # # # # # # # # # # # # # #

def main():
    ctr("bible.txt", "output.txt", 4, "jack".encode(), "-e")
    ctr("output.txt", "mirror.txt", 4, "jack".encode(), "-d")

            
if __name__ == "__main__":
    main()
