from secret import message
from functools import reduce
from hashlib import sha256
from Crypto.Util.number import bytes_to_long
import socketserver
import signal
import string 
import random

def dec2blist(decimal, length):
    """
    Converts an integer to a binary list of a specified length, filling zeros on the left if necessary.
    """
    bitlist = []
    while decimal > 0:
        bit = decimal % 2
        bitlist.append(bit)
        decimal //= 2
    bitlist.reverse()
    if len(bitlist) < length:
        bitlist = [0] * (length - len(bitlist)) + bitlist
    return bitlist

def bake(list1,list2):
    return reduce((lambda x,y: x ^ y) , list(map(lambda x, y: x and y, list1, list2)) )

def send_a_chocolate(self):
    assert len(bin(bytes_to_long(message)))-2==511
    dough=bytes_to_long(message)
    chocolate_jam=random.getrandbits(512)
    cookie=bake(dec2blist(dough, 512), dec2blist(chocolate_jam,512))
    self.send(f"[+] The lucky sticker reads ({cookie},{hex(chocolate_jam)}). Yummy!\n".encode())


class Task(socketserver.BaseRequestHandler):
    def _recvall(self):
        BUFF_SIZE = 2048
        data = b''
        while True:
            part = self.request.recv(BUFF_SIZE)
            data += part
            if len(part) < BUFF_SIZE:
                break
        return data.strip()

    def send(self, msg, newline=True):
        try:
            if newline:
                msg += b'\n'
            self.request.sendall(msg)
        except:
            pass

    def recv(self, prompt=b'[-] '):
        self.send(prompt, newline=False)
        return self._recvall()

    def proof_of_work(self):
        table = string.ascii_letters+string.digits
        proof = (''.join([random.choice(table)for _ in range(20)])).encode()
        sha = sha256(proof).hexdigest().encode()
        self.send(b"[+] sha256(XXXX+" + proof[4:] + b") == " + sha )
        XXXX = self.recv(prompt = b'[+] Plz Tell Me XXXX :')
        if len(XXXX) != 4 or sha256(XXXX + proof[4:]).hexdigest().encode() != sha:
            return False
        return True

    def sendmenu(self,coins):
        self.send(f"[+] Welcome to Flag Vending Machine! You have {coins} coins available. \
Please press the number to make your choice!\n[1] Buy a chocolate cookie\n[2] Top-up silver coins \n[3] Exit\n".encode())

    def handle(self):
        signal.alarm(233)

        coins=random.randint(503,508)#Well ... Easy mode

        self.send(b"Chocolate Fortune Cookie For Sale! You need to complete the Proof of work first!")
        proof = self.proof_of_work()
        if not proof:
            self.request.close()
        
        while(coins):
            self.sendmenu(coins)
            try:
                choice=int(self.recv().decode())
            except Exception as e:
                self.send(str(e).encode())
                continue

            if (choice == 1 and coins > 0):
                coins -= 1
                send_a_chocolate(self)
            elif (choice == 2):
                self.send(b"[+] Error: The service is currently unavailable, please try again later :(\n")
            elif (choice == 3):
                self.send(b"[+] Bye!\n")
                self.request.close()
            else:
                self.send(b"[+] Please send 1/2/3 to make your choice!\n")

        self.send(b"[+] Looks like you\'re out of coins... See you next time! Good luck!\n")
        self.request.close()

class ThreadedServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

class ForkedServer(socketserver.ForkingMixIn, socketserver.TCPServer):
    pass

if __name__ == "__main__":
    HOST, PORT = '0.0.0.0', 10007
    server = ForkedServer((HOST, PORT), Task)
    server.allow_reuse_address = True
    print(HOST, PORT)
    server.serve_forever()