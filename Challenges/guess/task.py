from secret import flag
from functools import reduce
from Crypto.Util.number import getPrime
from hashlib import sha256
import socketserver
import signal
import string 
import random
import gmpy2

N_SIZE=52
MOD_SIZE=50

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

def generate_prime_list(listlen:int,q:int):
    primes = []
    while len(primes) < listlen:
        n = random.randint(2, q-1)
        if gmpy2.is_prime(n):
            primes.append(n)
    return primes

def verify(prime_list,key,q,product):
    choice=dec2blist(key,len(prime_list))
    elements = [prime_list[i] for i in range(len(prime_list)) if choice[i]]
    newproduct = reduce((lambda x, y: x * y % q), elements)
    return product==newproduct

def product_mod_q(prime_list,q):
    l=len(prime_list)
    elements = random.sample(prime_list,random.randint(l//2,l))
    product = reduce((lambda x, y: x * y % q), elements)
    return product

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

    def handle(self):
        signal.alarm(233)

        self.send(b"Welcome to Guess Frenzy! You need to complete the Proof of work first!")
        proof = self.proof_of_work()
        if not proof:
            self.request.close()
        
        q=getPrime(MOD_SIZE)
        prime_list=generate_prime_list(N_SIZE,q)

        self.send(b"Alice have some primes:\n"+str(prime_list).encode()+b"\n")
        
        product=product_mod_q(prime_list,q)
        self.send(f"She picks some and multiplies them ,then mod {q} ,the product is {product}".encode())

        self.send(b"You should guess Alice's choice as the key to get the flag!\n")
        
        for _ in range(5):
            try:
                key=int(self.recv(b"[-] key=\n").decode())
            except Exception as e:
                self.send(str(e).encode())

            if(verify(prime_list,key,q,product)):
                self.send(f"Congratulations! Here's the flag for you \n{flag}".encode())
                self.request.close()
            else:
                self.send("Nope! Try again!".encode())

        self.send("Bye!")

class ThreadedServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

class ForkedServer(socketserver.ForkingMixIn, socketserver.TCPServer):
    pass

if __name__ == "__main__":
    HOST, PORT = '0.0.0.0', 10001
    server = ForkedServer((HOST, PORT), Task)
    server.allow_reuse_address = True
    print(HOST, PORT)
    server.serve_forever()