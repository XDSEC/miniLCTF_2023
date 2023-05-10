import random
from Crypto.Util.number import *

def generate_prime(bit_length):
    while True:
        a = random.getrandbits(bit_length//2)
        b = random.getrandbits(bit_length//2)

        if b % 3 == 0:
            continue

        p = a ** 2 + 3 * b ** 2
        if p.bit_length() == bit_length and p % 3 == 1 and isPrime(p):
            return p

def point_addition(P, Q, mod):
    m, n = P
    p, q = Q

    if p is None:
        return P
    if m is None:
        return Q

    if n is None and q is None:
        x = m * p % mod
        y = (m + p) % mod
        return (x, y)

    if n is None and q is not None:
        m, n, p, q = p, q, m, n

    if q is None:
        if (n + p) % mod != 0:
            x = (m * p + 2) * inverse(n + p, mod) % mod
            y = (m + n * p) * inverse(n + p, mod) % mod
            return (x, y)
        elif (m - n ** 2) % mod != 0:
            x = (m * p + 2) * inverse(m - n ** 2, mod) % mod
            return (x, None)
        else:
            return (None, None)
    else:
        if (m + p + n * q) % mod != 0:
            x = (m * p + (n + q) * 2) * inverse(m + p + n * q, mod) % mod
            y = (n * p + m * q + 2) * inverse(m + p + n * q, mod) % mod
            return (x, y)
        elif (n * p + m * q + 2) % mod != 0:
            x = (m * p + (n + q) * 2) * inverse(n * p + m * q + r, mod) % mod
            return (x, None)
        else:
            return (None, None)

def special_power(P, a, mod):
    res = (None, None)
    t = P
    while a > 0:
        if a & 1:
            res = point_addition(res, t, mod)
        t = point_addition(t, t, mod)
        a >>= 1
    return res

def random_padding(message, length):
    pad = bytes([random.getrandbits(8) for _ in range(length - len(message))])
    return message + pad