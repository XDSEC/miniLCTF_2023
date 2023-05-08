import random as rd
str_list = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
'''
eg: enc [0] = enc[0] ^ flag[0]
a, a, r0, r0, b, r0, b, b, r1, r1, a, r1, r0, r1, a,

a: 98~110 (flag)
b: 122~110 (flag)
r0: 66~90(reg)
r1: 66~90(reg)
'''
f = open('1.txt','w')
s1 = 'void '
s2 = '()\n{\n   strcpy(string, "'
s3 = '");\n}\n'
d_a = 97
d_b = 122
t = open('t.txt', 'w')
for j in range(13):
    w1 = ''
    w2 = ''
    r0 = chr(rd.randint(66, 90))
    r1 = chr(rd.randint(66, 90))
    a = chr(d_a)
    b = chr(d_b)
    w1 = a + a + r0 + r0 + b + r0 + b + b + r1 + r1 + a + r1 + r0 + r1 + a
    t.write('"'+w1+'",\n')
    for i in range(15):
        w2 += str_list[rd.randint(0, 51)]
    w = s1 + w1 + s2 + w2 + s3
    f.write(w)
    d_a += 1
    d_b -= 1

d_a = 110
d_b = 95
for j in range(13):
    w1 = ''
    w2 = ''
    r0 = chr(rd.randint(66, 90))
    r1 = chr(rd.randint(66, 90))
    a = chr(d_a)
    b = chr(d_b)
    w1 = a + a + r0 + r0 + b + r0 + b + b + r1 + r1 + a + r1 + r0 + r1 + a
    t.write('"'+w1+'",\n')
    for i in range(15):
        w2 += str_list[rd.randint(0, 51)]
    w = s1 + w1 + s2 + w2 + s3
    f.write(w)
    d_a += 1

f.close()
t.close()