def caesar(plaintext):
    ciphertext = ""
    for char in plaintext.lower():
        new_char = 0
        if ord(char) >= 97 and ord(char) <= 122:
            new_char = ord(char) + 3
            if new_char > 122:
                new_char -= 26
        ciphertext += chr(new_char)
    return ciphertext.upper()

def main():
    plaintext = input("Please enter plaintext to be encrypted: ")
    ciphertext = caesar(plaintext)
    print("Ciphertext: " + ciphertext)

if __name__ == "__main__":
    main()
