import socket
import struct

def encrypt(pt: str, shift: int):
    cipher = ""
    for c in pt:
        if ('a' <= c <= 'z'):
            cipher += chr((ord(c) - ord('a') + shift) % 26 + ord('a'))
        elif ('A' <= c <= 'Z'):
            cipher += chr((ord(c) - ord('A') + shift) % 26 + ord('A'))
        else:
            cipher += c
    return cipher


def send_command(ip, port, command, shift_val):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ip, port))
        cipher_text = encrypt(message, shift_val) 
        cipher_bytes = cipher_text.encode('utf-8')
        print (cipher_text)
        header = struct.pack('!I', len(cipher_bytes) + 1)
        tail = struct.pack('B', shift_val)
        packet = header + cipher_bytes + tail
        s.sendall(packet)

if __name__ == "__main__":
    message = "This is a test message"
    print (message)
    send_command('127.0.0.1', 12578, message, 15)