'''
f = 'QwQ_s0oOo_simple_LlVm_P4s5'
for i in range(13):
	print(ord(f[i])^ord(f[25-i]),end=', ')
for i in range(13,26):
	print(ord(f[i])^0xff,end=', ')
'''

enc = [100, 4, 101, 15, 44, 93, 57, 35, 35, 0, 22, 5, 29, 143, 147, 154, 160, 179, 147, 169, 146, 160, 175, 203, 140, 202]

for i in range(13,26):
	enc[i]^=0xff
for i in range(13):
	enc[i]^=enc[25-i]
print(bytes(enc))