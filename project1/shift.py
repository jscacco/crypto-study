#*****************************************************************************
# Name: Jack Scacco
# Date: 9/6/19
# File: shift.py
# Assignment: Project1
# Purpose: Implement the 
#*****************************************************************************

import random
import re
import sys

# A global list of letter frequencies, where LETTER_FREQ[i]
# is the frequency of the i+1-th letter of the alphabet.
# Source: Some university's website
LETTER_FREQ = [
    .08167, # a
    .01492, # b
    .02782, # c
    .04253, # d
    .12702, # e
    .02228, # f
    .02015, # g
    .06094, # h
    .06966, # i
    .00153, # j
    .00772, # k
    .04025, # l,
    .02406, # m,
    .06749, # n,
    .07507, # o,
    .01929, # p,
    .00095, # q,
    .05987, # r,
    .06327, # s,
    .09056, # t,
    .02758, # u,
    .00978, # v,
    .02360, # w,
    .00150, # x,
    .01974, # y,
    .00074, # z
]

# The sum of the squares of each letter's frequency
GOAL_VAL = 0.06549669949999998


def get_op():
    """Gets the operation to be computed from the command line"""
    if len(sys.argv) < 2:
        print("Error! You must enter either '-e' or '-d' as the first argument on the command line.")
        return -1
    return sys.argv[1]


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
    

def write_to_file(filename, info):
    """Writes the string info to filename"""
    with open(filename, 'a') as f:
        f.write(info)

        
def is_letter(input):
    char = input.lower()
    return ord(char) >= ord('a') and ord(char) <= ord('z')


def get_valid_key():
    """Gets a valid key from the user (between 0-25) """
    key = None
    # We want to make sure they enter a value between 0-25
    while (key == None):
        key = input("Please enter the key (0-25): ")
        # Check to see if it is a number
        if (re.match("-?[0-9]+", key) == None):
            print("Please enter a number.")
            key = None
        # If it is a number, check to make sure it is a valid value.
        elif (int(key) < 0 or int(key) > 25):
            print("Please enter a number within the range [0-25].")
            key = None
        else:
            return(int(key))


def get_text():
    """Prompts the user for text to be interpreted."""
    if get_op() == "-e":
        return input("Please enter plaintext to be encrypted: ")
    return input("Please enter ciphertext to be decrypted: ")

def get_info():
    """Prompts the user for text and key."""
    # We don't care much what they enter, as we only encrypt letters. (Check in encrypt())
    plaintext = get_text()
    key = get_valid_key()
    return (plaintext, key)


def encrypt(plaintext, key):
    """Takes plaintext and key, shifts each letter according to key.""" 
    ciphertext = ""
    for char in plaintext.lower():
        # Initialize to zero so we don't add anything if not a letter
        new_char = 0
        # Ensure it is a letter
        if is_letter(char):
            new_char = ord(char) + key
            if new_char > ord('z'):
                new_char -= 26 # This is essentially the same as (mod 26)
            ciphertext += chr(new_char)
    return ciphertext.upper()


def do_encrypt():
    """Gets information from the file specified and encrypts it"""
    # Check to see if there is an input file. If not, prompt the user for info.
    if get_input_file() == -1:
        (plaintext, key) = get_info()
        print(encrypt(plaintext, key))
    else:
        # Get the key and output file (out_file will be -1 if there isn't one)
        if get_op() == '-r':
            key = random.randint(0,26)
        else:
            key = get_valid_key()
        out_file = get_output_file()
        # Clear the output file if we have one
        if out_file != -1:
            open(out_file, 'w').write("")
        # Read through the file and encrypt one line at a time
        with open(get_input_file(), 'r') as f:
            current_line = f.readline()
            while current_line:
                ciphertext = encrypt(current_line, key)
                # Write the encrypted line to the appropriate location
                if out_file == -1:
                    sys.stdout.write(ciphertext)
                else:
                    write_to_file(out_file, ciphertext)
                # Read the next line
                current_line = f.readline()
        if out_file == -1:
            print()

                
def build_freqs():
    """Reads from the input and calculates the frequency of each letter."""
    total_letters = 0 # Keeps track of the total letters
    frequencies = [] # Keeps track of the frequencies of each letter
    # populate the array with zeros
    for num in range(26):
        frequencies.append(0.0)

    # First, we need to gather the occurence of each character in the input
    input_file = get_input_file()
    # If no input file was submitted, calc the occurences from the standard input
    if input_file == -1:
        ciphertext = get_text()
        for char in ciphertext:
            this_char = char.lower()
            if is_letter(this_char):
                total_letters += 1
                # Update the freqs (set 'a' to 0 for indexing)
                frequencies[ord(this_char) - ord('a')] += 1
        return frequencies
    # Otherwise, read one line at a time from the input and update occurences
    else:
        with open(input_file, 'r') as f:
            current_line = f.readline()
            while current_line:
                for char in current_line:
                    this_char = char.lower()
                    if is_letter(this_char):
                        total_letters += 1
                        # Update the freqs (set 'a' to 0 for indexing)
                        frequencies[ord(this_char) - ord('a')] += 1
                current_line = f.readline()

    # Lastly, divide the occurences by the total letters to get the frequency!
    for i in range(26):
        frequencies[i] = frequencies[i] / total_letters
    return frequencies


def calc_magic_num(freqs, key):
    """Takes an array of frequencies and returns the sum of each freq*real_freq."""
    # This is the method outlined in Introduction to Modern Cryptography
    magic_num = 0
    for i in range(26):
        magic_num += LETTER_FREQ[i]*freqs[(i + key)%26]
    return magic_num


def determine_key(freqs):
    """Builds freqs, tries all keys in calc_magic_num, returns the best key"""
    best_key = 0
    best_val = calc_magic_num(freqs, 0)
    for k in range(26):
        temp_val = calc_magic_num(freqs, k)
        if abs(GOAL_VAL - temp_val) < abs(GOAL_VAL - best_val):
            best_val = temp_val
            best_key = k
    return best_key


def decrypt(ciphertext, key):
    """Takes plaintext and key, unshifts each letter according to key.""" 
    plaintext = ""
    for char in ciphertext.lower():
        # Initialize to zero so we don't add anything if not a letter
        new_char = 0
        # Ensure it is a letter
        if is_letter(char):
            new_char = ord(char) - key
            if new_char < ord('a'):
                new_char += 26 # This is essentially the same as (mod 26)
            plaintext += chr(new_char)
    return plaintext.lower()


def do_decrypt():
    """Puts it all together to decrypt the message and print it out"""
    cipher_freqs = build_freqs()
    cipher_key = determine_key(cipher_freqs)

    input_file = get_input_file()
    out_file = get_output_file()
    # Clear the output file if we have one
    if out_file != -1:
        open(out_file, 'w').write("")
    # Read through the file and decrypt one line at a time
    with open(get_input_file(), 'r') as f:
        current_line = f.readline()
        while current_line:
            plaintext = decrypt(current_line, cipher_key)
            # Write the decrypted line to the appropriate location
            if out_file == -1:
                sys.stdout.write(plaintext)
            else:
                write_to_file(out_file, plaintext)
            # Read the next line
            current_line = f.readline()
    if out_file == -1:
        print()

def main():
    op = get_op()
    if op == "-e" or op == '-r':
        do_encrypt()
    elif op == '-d':
        do_decrypt()


if __name__ == "__main__":
    main()
