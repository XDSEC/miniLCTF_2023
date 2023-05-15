# miniL 2023 Crypto Official Writeup

出题组：Deebato, b0y4nz, LGLJ,Polarnova

## Giveaway

签到题，本题考察 $GF(2)$ 下的线性代数。

```python
def bake(list1,list2):
    return reduce((lambda x,y: x ^ y) , list(map(lambda x, y: x and y, list1, list2)) )
```

`bake`函数计算含有`flag`的`dough`向量和一个随机数向量`chocolate_jam`的点积`cookie`。我们每次请求可以得到一个`chocolate_jam`和对应`cookie`的值。一共可以得到503~508个方程组成矩阵：

$$
A_{\text{jam}}x=b_\text{cookie}
$$

由于 $A$ 矩阵不满秩，我们可以通过求一个特解和（右）零空间来求出所有可能的 $b$ ，遍历一遍（规模 $2^4$ ~ $2^9$ ，搜索空间并不大）即可。

```py
from pwn import *
from hashlib import sha256
import string 
from sage.all import *
import re
from Crypto.Util.number import long_to_bytes

HOST, PORT = XXXXX, XXXXX

table = string.ascii_letters+string.digits

def proof_of_work(s,tsha,sh):
    for i in table:
        for j in table:
            for k in table:
                for l in table:
                    proof=i+j+k+l+s
                    if(sha256(proof.encode()).hexdigest()==tsha):
                        sh.sendline((i+j+k+l).encode())
                        return

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

def blist2dec(bitlist):
    """
    Converts a binary list to an integer.
    """
    decimal = 0
    for bit in bitlist:
        decimal = decimal * 2 + bit
    return decimal

def work():
    sh=remote(HOST, PORT)

    pres=sh.recvuntil(b'sha256(XXXX+',drop=True)
    s=sh.recvuntil(b') == ',drop=True).decode()
    tsha=sh.recvuntil(b'\n',drop=True).decode()

    proof_of_work(s,tsha,sh)

    b = []
    M = []
    try:
        while True:
            resp = sh.recvuntil(b'Exit\n', drop=True)

            sh.sendline(b'1')
            
            resp = sh.recvuntil(b'Yummy!\n', drop=True)

            match = re.search(r'\((\d+),0x([0-9a-f]+)\)', resp.decode())
            x = int(match.group(1))
            c = int(match.group(2), 16)

            b.append(x)
            M.append(dec2blist(c,512))
    except EOFError:
        pass
    
    F=GF(2)
    M=Matrix(GF(2),M)
    b=vector(b)

    Ker=M.right_kernel()
    sp=M.solve_right(b)

    r=Ker.dimension()
    print("Kernel dimension=",r)

    for i in range(0,1<<r):
        tmp=sp+Ker[i]
        flag=blist2dec(list(tmp.change_ring(ZZ)))
        if(len(bin(flag))-2==511):
            flag=long_to_bytes(flag)
            if(flag[:4]==b'mini'):
                print(flag)

work()
```

非预期（这下真giveaway了）：

由于出题人失误每次`dough`向量没变，本题是交互题，连两次服务器就凑够512个方程了...直接求逆即可。


## not RSA

附件`not RSA.py`:

```python
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
```

`task.py`:

```python
from not_RSA import *
from secret import FLAG
from Crypto.Util.number import *

p, q = generate_prime(512), generate_prime(512)
n = p * q
phi = (p**2 + p + 1) * (q **2 + q + 1)


d = getPrime(276)
e = inverse(d, phi)
tmp = getPrime(469)
p0 = p + tmp

pt1, pt2 = random_padding(FLAG[:len(FLAG)//2]+b'#',127), random_padding(FLAG[len(FLAG)//2:]+b'#', 127)

m = (bytes_to_long(pt1), bytes_to_long(pt2))
c = special_power(m, e, n)

print(f"c = {c}")
print(f"n = {n}")
print(f"e = {e}")
print(f"p0 = {p0}")
"""
c = (99256707703226697226473841185259891785249728547098403329816239886722383460401685922071518907503131597586071155779535217957728713395973126772467862964939878117327514388525570332680833383088079421676354296281469250418264543833996288111463346112204924207384792233847819302304599120532752360882845527395569869907, 22655358796075424487044406387957775030913109276145369023351200306937368259451273503046617611110582153415768404632774105652118572620829335937285604752846386248015325031053581797994703852239663030464437053164169557845378554791579176562234005623449839772205446210182378591192462742536627314113540667791362602148)
n = 103255210447201501371417366314698617128571899104178153644502440939179707560694633551313596814867085426522157527557873368089757491021794381392940645658031944937376477744644393844781470315770893253591718873148298034783254899285894192568113349056391974188706470251298810392910953025658474958447750644663120809161
e = 9583844349143763216577056030562049770207021495053166931622586942299664240152962005166123845294740910682002626836306629541171186155613228080333603389256263599077945629292275075204305859845966709343064493385561097725880568307482154382068336740221552489762224156862649726139521041709368241009505934006275050727466118506845275244191749935821428533956123008950814817446865191293160484499980805375127998664220058827288306710393417375308743911041896280674963812278295706552776413678414777273337864851457733395645762523718466931204393235542589232058708808567310860905450262329471814920834990514208969596816366119172152233564
p0 = 8989358155637718504643502172367267711566059539795670198816016094340804453065250030031846883560365745256555868280844477116616537047437144736243403626554094
"""
```

出题人的想法：

本来想出一个论文题。

根据题目描述，首先明确虽然考的不是RSA，但却和RSA关系密切。

先根据附件查资料，查到题目中的密码系统是**Murru-Saettone cryptosystem**，类RSA密码系统。

> Murru-Saettone密码系统是一种类似rsa的密码系统，由与佩尔方程的一般三次形式相关的非标准乘积组成。

然后由`task.py`的形式，先给了较小的`d`，类似于RSA的**wiener attack**，因此翻阅相关论文：

找到以下文章：

[A Wiener-type attack on an RSA-like cryptosystem constructed from cubic Pell equations](https://www.sciencedirect.com/science/article/abs/pii/S030439752100390X)

[Improving small private exponent attack on the Murru-Saettone cryptosystem](https://www.sciencedirect.com/science/article/abs/pii/S0304397522003085)

根据论文的介绍，得知可以通过**连分数**对此进行攻击，同时也可以找到符合题目的改进的连分数攻击——第二篇论文的第三种攻击。

证明方式在此略过，详情可参考论文：

连分数攻击代码：

```python
from not_RSA import *
from Crypto.Util.number import *
from tqdm import trange
from sage.all import *
c = (99256707703226697226473841185259891785249728547098403329816239886722383460401685922071518907503131597586071155779535217957728713395973126772467862964939878117327514388525570332680833383088079421676354296281469250418264543833996288111463346112204924207384792233847819302304599120532752360882845527395569869907, 22655358796075424487044406387957775030913109276145369023351200306937368259451273503046617611110582153415768404632774105652118572620829335937285604752846386248015325031053581797994703852239663030464437053164169557845378554791579176562234005623449839772205446210182378591192462742536627314113540667791362602148)
n = 103255210447201501371417366314698617128571899104178153644502440939179707560694633551313596814867085426522157527557873368089757491021794381392940645658031944937376477744644393844781470315770893253591718873148298034783254899285894192568113349056391974188706470251298810392910953025658474958447750644663120809161
e = 9583844349143763216577056030562049770207021495053166931622586942299664240152962005166123845294740910682002626836306629541171186155613228080333603389256263599077945629292275075204305859845966709343064493385561097725880568307482154382068336740221552489762224156862649726139521041709368241009505934006275050727466118506845275244191749935821428533956123008950814817446865191293160484499980805375127998664220058827288306710393417375308743911041896280674963812278295706552776413678414777273337864851457733395645762523718466931204393235542589232058708808567310860905450262329471814920834990514208969596816366119172152233564
p0 = 8989358155637718504643502172367267711566059539795670198816016094340804453065250030031846883560365745256555868280844477116616537047437144736243403626554094

M = n^2 - ((n+1)//2)^2 - n + 1 + (p0 + n//p0 + (n+1)//2)^2

def factorize_extend3(e, N, M):
    f = continued_fraction(e/(M+1))
    try:
        Len = min(len(f),10000)
    except:
        Len = 10000
    for i in trange(Len+1):
        a = f.numerator(i)
        b = f.denominator(i)
        if gcd(a,e*b-1) == a and a != 1:
            S = var('S')
            phi_N = (e*b-1)//a
            eqn = S^2 + (N+1)*S + N^2 - N + 1 - phi_N
            sols = solve(eqn == 0, S, solution_dict=True)
            for sol in sols:
                tmp = sol[S]
                if tmp.is_integer():
                    x = var('x')
                    eqn1 = x^2 - tmp*x + N
                    sols1 = solve(eqn1 == 0, x, solution_dict=True)
                    p = int(sols1[0][x])
                    if gcd(p,N) == p:
                        return (b,p,N//p)
    return None
d,p,q = factorize_extend3(e,n,M)
assert p*q == n
m = special_power(c, d, n)
m = (long_to_bytes(m[0]),long_to_bytes(m[1]))
print(m[0][:m[0].find(b'#')]+m[1][:m[1].find(b'#')])
```

flag:

```python
FLAG = b"miniLCTF{CoNt1nu4d_FrActiOn_1s_3o_E@s7_foR_y0u!}"
```



非预期：

其实也算是预期解，类似于RSA，同样可以通过**coppersmith**进行攻击。

[[羊城杯] 2022年羊城杯网络安全大赛 Crypto方向 团队writeup NovelSystem2](https://zhuanlan.zhihu.com/p/561322960)

听说稍微魔改一下可以不用`p0`直接求出来。

出题人自锤🔨赛前检索的时候没有检索到。。。（因为没有提到这个系统名字/(ㄒoㄒ)/~~

具体代码参考选手wp

`not RSA.py (Murru-Saettone cryptosystem)`代码参考：

[Attacking a Variant of the RSA Cryptosystem](https://github.com/infossm/infossm.github.io/blob/d36b0c521e42c2a419415fbc00864a730c08f16b/_posts/2021-11-21-Attacking-a-Variant-of-the-RSA-Cryptosystem.md)



## guess

题目给出一个大小为 $n$ 的全是素数的集合 $S$ ，随机取一个子集 $S'$，给出计算这个子集中的数的乘积模 $q$ 的结果，要求恢复所选的子集 $ S'$ 。

$$
\prod_{p \in S'}p = r\pmod q
$$

首先等式两边同时取离散对数，将其化为一个子集和问题（背包问题）:

$$
\sum_{p \in S'}\log_g p =\log_g r\pmod {q-1}
$$

其中 $g$ 是模 $q$ 乘群的一个生成元。为了方便书写，对于 $p \in S$  记 $\log_g p_i =m_i$ ,  $\log_g r = s$

构造优化的背包格

$$
\begin{bmatrix}
q-1  &  &  &  &  & \\
 m_1 & 2 &  &  &  & \\
 m_2 &  & 2 &  &  & \\
 \vdots &  &  & \ddots  &  & \\
  m_n &  &  &  & 2 & \\
  s& 1 &  1& \cdots & 1 &1
\end{bmatrix}
$$

目标向量：

$$
v_u=(0,\pm 1,\pm 1,\cdots,\pm 1,1)
$$

 $v_u$ 的大小不在LLL算法能确保规约出的向量大小范围之内，使用BKZ (blocksize=22~30) 大概率能找到目标向量。此外，将第一列全部乘一个大整数可以迫使规约算法将第一个元素消成0。

根据得到的向量即可恢复子集 $S'$。

```python
from pwn import *
from hashlib import sha256
import string 
from tqdm import tqdm
from sage.all import *

HOST, PORT = XXXXX ,XXXXX


def blist2dec(bitlist):
    """
    Converts a binary list to an integer.
    """
    decimal = 0
    for bit in bitlist:
        decimal = decimal * 2 + bit
    return decimal

def verify(lst,ans,q,rp):
    ans=ans[1:]
    #print(ans)
    elements = [lst[i] for i in range(len(lst)) if ans[i]]
    product = reduce((lambda x, y: x * y % q), elements)
    #print(elements)
    #print(rp,product)
    return rp==product

table = string.ascii_letters+string.digits

def proof_of_work(s,tsha,sh):
    for i in table:
        for j in table:
            for k in table:
                for l in table:
                    proof=i+j+k+l+s
                    if(sha256(proof.encode()).hexdigest()==tsha):
                        sh.sendline((i+j+k+l).encode())
                        return

def work():
    sh=remote(HOST, PORT)

    pres=sh.recvuntil(b'sha256(XXXX+',drop=True)
    s=sh.recvuntil(b') == ',drop=True).decode()
    tsha=sh.recvuntil(b'\n',drop=True).decode()

    proof_of_work(s,tsha,sh)
    s=sh.recvuntil(b'primes:',drop=True)
    lst=(sh.recvuntil(b']',drop=True)[2:].decode())
    prime_list= [int(i) for i in lst.split(",")]


    s=sh.recvuntil(b'then mod ',drop=True)
    q=int(sh.recvuntil(b' ,the product is ',drop=True).decode())

    rp=int(sh.recvuntil(b'\n',drop=True).decode())

#    print(prime_list,"\n","q=",q,"product=",rp)

    #discrete log
    F = GF(q)
    g = F.primitive_element()
    logprime_list = []
    for i in tqdm(prime_list):
        e = F(i).log(g)
        logprime_list.append(e)

    r=F(rp).log(g)


    #construct the Lattice matrix
    M=[]
    lstlen=len(logprime_list)+1

    M.append([(q-1)]+ [0]*lstlen)

    for i in range(len(logprime_list)):
        temp=[0]*(lstlen+1)
        temp[0]=logprime_list[i]
        temp[i+1]=2
        M.append(temp)

    M.append([r]+lstlen*[1])
    M=Matrix(ZZ,M)

    print("===BKZ start====")
    L=M.BKZ(block_size=30)
    print("===BKZ end====")

    for i in range(L.dimensions()[0]):
        if(L[i][-1]==1):
            ans=list((L[i]+vector([-1]*L.dimensions()[1]))/-2)[:-1]
            #print(ans)
            if(verify(prime_list,ans,q,rp)):
                key=blist2dec(ans[1:])
                print("key=",key)
                sh.sendline(str(key).encode())
                sh.interactive()

    sh.close()

while True:
    work()
```



## MiniFFI

两解

**题目描述**  miniffi，Algebra是密码学中基础的数学原理，学会使用sagemath并尝试解决这一有限域有关的问题。
**hint1**:  ffi代表有限域同构(Finite Field Isomorphism)，尝试去构造此同态映射的逆映射。

### 理论前提

#### 有限域构造

此题中`R`是定义的一元多项式环，设为 $F_q[x]$  ，并且 $f(x),F(x)$ 是 $F_q[x]$ 中的`128`次不可约多项式，因此 $ F_q[x]/(f(x)) $ 以及 $F_q[x]/(F(x))$ 是含有 $q^n$ 个元素的有限域，并且它的每一个元素可以唯一地表示成

$$
c_0+c_1u+c_2u^2+...+c_{n-1}u^{n-1}
$$

对 $f(x)$ ，其中 $c_i\in F_q,u=x+f(x)$ 。如果 $ f(x)=a_0+a_1x+...+a_nx^n $ ，则满足 $a_0+a_1u+a_nu^n=0$ 。

实际上，对于实数域来说，假设`R[x]` ，取一个二次不可约多项式 $m(x)=x^2+1$ ，做商环 $R[x]/(x^2+1)$ ，则他是域，并且与复数域同构，原因就是域中元素可以表示成 $\{a+bu|a,b\in R\}$  ，而 $u^2+1=0$  。

设 $q=p^n$ ，p 为素数，则q元有限域一定存在，并且任意两个q元有限域都是同构的。

对于本题来说， $f(x),F(x)$ 均是 $F_q$ 上的不可约多项式，那么 $F_q[x]/f(x)$ ， $F_q[x]/F(x)$ 都是 $q^n$ 个元素的有限域，并且同构，而满足 $f(\phi(x))\ mod\ F(x)=0$  ，实际上令 $u=\phi(x)$ ，即为在有限域 $F_q[x]/F(x)$ 中 $f(u)=0$ ，故 $\phi(x)$ 为 $F_q[x]/f(x)\rightarrow F_q[x]/F(x)$ 的同构映射。

加密函数： $C(x)=( 2*\phi(x)*r(\phi(x)) + m(\phi(x)) ) \ mod\ F(x)$

其中 $m(x),r(x)$ 系数为0或1，只需要求得其逆映射使得  $\psi(\phi(x))\equiv y\ (mod\ F(y))$ 即 $\phi(\psi(x))\equiv x\ (mod\ f(x))$，可以得到 $C(\psi(x))=2*x*r(x)+m(x) \ (mod\ f(x))$。然后换到二元域上即可得到 $m(x)$。

### 解题方法

上述问题转化为如何求取这个同态映射的逆映射：

总体上太概有三种方法，exp将给出利用线性代数解决方法，其余方法请自行尝试。

1、在有限域 $F_q[x]/(f(x))$中求得 $F(y)$的解，需要注意的是，因为会有n个不同的根，需要找到与上述映射相对应的逆映射。

2、在有限域 $F_q[x]/(f(x))$中求得 $\phi(y)-x$的根。

3、使用线性代数的方法。

方法三：

不妨设 $\psi(x)=c_0+c_1x+...+c_{n-1}x^{n-1}$，我们的目标是构造 $\psi(\phi(x))\equiv y\ (mod\ F(y))$。

$$
\sum_{i=0}^{n-1}c_i\phi(y)^i \equiv y\ (mod\ F(y))
$$

$$
\sum_{i=0}^{n-1}c_i\phi(y)^i=\sum_{i=0}^{n-1}c_i\sum_{j=0}^{n-1}a_{ij}y^j=\sum_{j=0}^{n-1}(\sum_{i=0}^{n-1}a_{ij}c_i)y^j\ (mod\ F(y))
$$



因此只需要当 $j=1$时 $\sum_{i=0}^{n-1}a_{ij}c_i=1$，否则  $\sum_{i=0}^{n-1}a_{ij}c_i=0$  即可。

**exp**

```python
from  Crypto.Util.number import *
from Crypto.Cipher import AES

p = 41
n = 128
R = PolynomialRing(GF(p),'x')
x = R.gen()
f = x^128 + 25*x^8 + 36*x^7 + 8*x^6 + 11*x^5 + 14*x^4 + 17*x^3 + 6*x^2 + 23*x + 4
F = x^128 + 4*x^59 + 8*x^58 + 14*x^57 + 5*x^56 + 17*x^55 + 13*x^54 + 15*x^53 + 17*x^52 + 31*x^50 + 18*x^49 + 5*x^48 + 3*x^47 + 3*x^46 + 35*x^45 + 9*x^44 + 38*x^43 + 14*x^42 + 37*x^41 + 16*x^40 + 32*x^39 + 6*x^38 + 18*x^37 + 27*x^36 + 39*x^35 + 25*x^34 + 37*x^32 + 2*x^31 + 6*x^30 + 18*x^29 + 5*x^28 + 25*x^27 + 28*x^25 + 14*x^24 + 32*x^23 + 14*x^22 + 17*x^21 + 12*x^20 + 36*x^19 + 25*x^18 + 36*x^17 + 34*x^16 + 27*x^15 + 14*x^14 + 19*x^13 + 14*x^12 + 14*x^11 + 15*x^10 + 20*x^9 + 24*x^8 + 18*x^7 + 5*x^6 + 19*x^5 + 34*x^4 + 40*x^3 + 32*x^2 + 10*x + 38
phi = 23*x^126 + 38*x^125 + 15*x^124 + 19*x^123 + 32*x^122 + 3*x^121 + 15*x^120 + 26*x^118 + 3*x^117 + 5*x^116 + 34*x^115 + 31*x^114 + 37*x^113 + 26*x^112 + 7*x^111 + 40*x^110 + 23*x^109 + 15*x^108 + 5*x^107 + 8*x^106 + 5*x^105 + 38*x^104 + 22*x^103 + 26*x^102 + 24*x^101 + 25*x^100 + 28*x^99 + 12*x^98 + 2*x^97 + 26*x^96 + 6*x^95 + 29*x^94 + 2*x^93 + 22*x^92 + 23*x^90 + 31*x^89 + 12*x^88 + 23*x^87 + 14*x^86 + 29*x^85 + 29*x^84 + 28*x^83 + 36*x^82 + 22*x^81 + 39*x^80 + 31*x^79 + 9*x^78 + 2*x^76 + 32*x^75 + 22*x^74 + 34*x^73 + 34*x^72 + 16*x^71 + 19*x^70 + 35*x^69 + 35*x^68 + 23*x^67 + x^66 + 5*x^65 + 28*x^64 + 22*x^63 + 23*x^62 + 3*x^61 + 21*x^59 + 36*x^58 + 25*x^57 + 28*x^56 + 30*x^55 + 12*x^54 + 32*x^53 + 31*x^52 + 9*x^51 + 38*x^50 + 38*x^49 + 2*x^48 + 18*x^47 + 12*x^46 + 9*x^45 + 21*x^44 + 10*x^43 + 6*x^42 + 39*x^41 + 13*x^40 + 29*x^39 + x^38 + 30*x^36 + 22*x^35 + 35*x^34 + 17*x^33 + 14*x^32 + 2*x^31 + 24*x^30 + 37*x^29 + 8*x^28 + 6*x^27 + 15*x^26 + 5*x^25 + 17*x^24 + 22*x^23 + 14*x^22 + 37*x^21 + 28*x^20 + 4*x^19 + 5*x^18 + 39*x^17 + 12*x^16 + 19*x^15 + 30*x^13 + 39*x^11 + 2*x^10 + 9*x^9 + 37*x^8 + 18*x^7 + 5*x^5 + 15*x^4 + 9*x^3 + 19*x^2 + 23*x + 39
assert f(phi(x))%F(x) == 0

polys = []
for i in range(n):
    polys.append(pow(phi(x),i,F(x)))

monomials = [x**i for i in range(n)]
B = Matrix(Zmod(p), n)

for ii in range(1,n):
    for jj in range(n):
        if monomials[jj] in polys[ii].monomials():
            B[ii, jj] = polys[ii].monomial_coefficient(monomials[jj])

t = Matrix(GF(p), 1,n)
t[0,1] = 1;B[0,0] = 1

psi = t*B**(-1)
psi = tuple(list(psi)[0])
psi = R(psi)

C=30*x^127 + 28*x^126 + 20*x^125 + 28*x^124 + x^123 + 16*x^122 + 23*x^121 + 34*x^120 + 31*x^119 + 7*x^118 + 4*x^117 + 31*x^116 + 34*x^115 + 12*x^114 + 33*x^113 + x^112 + 4*x^111 + 17*x^110 + 7*x^109 + 17*x^108 + 16*x^107 + 9*x^106 + 35*x^105 + 7*x^104 + 20*x^103 + 24*x^102 + 20*x^101 + 17*x^100 + 13*x^99 + 39*x^98 + 16*x^97 + 23*x^96 + 36*x^95 + 25*x^94 + 12*x^93 + 40*x^92 + 19*x^91 + 36*x^90 + 17*x^89 + 40*x^88 + 23*x^87 + 29*x^86 + 29*x^85 + 13*x^84 + 5*x^83 + 31*x^82 + 14*x^81 + 23*x^80 + x^79 + 20*x^78 + 36*x^77 + 7*x^76 + 34*x^75 + 11*x^74 + 23*x^73 + 11*x^72 + 15*x^71 + 35*x^70 + 33*x^69 + 20*x^68 + 28*x^67 + 35*x^66 + 33*x^65 + 37*x^64 + 33*x^63 + 33*x^62 + 34*x^61 + 5*x^60 + 8*x^59 + 23*x^58 + 10*x^57 + 23*x^56 + 26*x^55 + 18*x^54 + 39*x^53 + 9*x^52 + 30*x^51 + 27*x^50 + x^49 + 8*x^48 + 35*x^47 + 33*x^46 + 2*x^44 + 12*x^43 + 2*x^42 + 16*x^41 + 39*x^40 + 13*x^39 + 33*x^38 + 5*x^37 + 36*x^36 + 26*x^35 + 34*x^34 + 34*x^33 + 16*x^32 + 29*x^31 + 32*x^30 + x^29 + 7*x^28 + 27*x^27 + 22*x^26 + 6*x^25 + 11*x^24 + 36*x^23 + 18*x^22 + 19*x^21 + 6*x^20 + 14*x^19 + 34*x^18 + 15*x^17 + 4*x^15 + 4*x^14 + 6*x^13 + 29*x^12 + 38*x^11 + 37*x^9 + 36*x^8 + 8*x^7 + 5*x^6 + 26*x^4 + 14*x^3 + 15*x^2 + 35*x + 5
cipher=b'\x1eO\xfe\xdd\xeeV\x04\xff\x9e\x9b\xadLbC]\x08\xb1\xf3e\x11U\xa4\xe7\xe2\x1bO\x84\xe52\xfd\xe0J&\xa3\xad8\x88\xbf{\x04&\xe87\xc6A62\xba'

c_1 = C(psi(x))
m_1 = c_1(x)%f(x) 
m1 = m_1.change_ring(Zmod(2))

t1 = ''.join(str(i) for i in m1.coefficients(sparse=False))
if len(t1) != 128:
    t1 += '0'*(128-len(t1))

key = int('0b'+t1 , 2)

aes = AES.new(long_to_bytes(key), mode=1)
print(aes.decrypt(cipher))

```



## curvesignin

分析point_addition得到，定一个元 $d$ 为1

$$
cy^2=dx^3+ax+b\\
cy^2=x^3+ax+b
$$

多个点可以求出来对应的 $a$ , $b$ , $c$ 是多少

然后设 $y_1=c^2dy$ , $x_1=cdx$ , 则 $  (c^2y)^2 = (cx)^3 + (c^2a)(cx)+(c^3b) $  ，即可替换

PH求出来之后是模了一次阶的，阶相较模数比较小，所以需要爆一下模了多少个

```python

from Crypto.Util.number import *
G = (543964449142803087188784, 288605946000947449279542)
Ps = [(615716520116903164238350, 815735349922061558569393), (256042693223413187255214, 400772499742719022770893), (620452972415969514798065, 660749148335795540667246), (118301218326000413235697, 338882909672963432813505), (793604064853039619406880, 93386361837996865025986)]
P = Ps[-1]
Ps = [G] + Ps
PR.<a,b,c,d> = PolynomialRing(ZZ)
fs = []
for P in Ps:
    x,y = P
    f = d*x^3 + a*x + b - c*y^2
    fs.append(f)
I = Ideal(fs)
'''
这里可以少给一个变元，由于我么发现f中每个系数都是变元，那么我们可以假定a,b,c,d中任意一个是1
那么我们就可以得到p的倍数
'''

'''Ps = [G] + Ps
PR.<a,b,c> = PolynomialRing(ZZ)
fs = []
for P in Ps:
    x,y = P
    f = x^3 + a*x + b - c*y^2
    fs.append(f)
I = Ideal(fs)'''

# sage: I.groebner_basis()
# [a + 1383761324420174433943441, b + 101839133990109714860886, c + 4911310444444537745356422, 5271017285827233379503222]
# sage: factor(5271017285827233379503222)
# 2 * 3 * 878502880971205563250537
# sage: -1383761324420174433943441%878502880971205563250537
# 373244437522236692557633
# sage: -101839133990109714860886%878502880971205563250537
# 776663746981095848389651
# sage: -4911310444444537745356422%878502880971205563250537
# 359706841382695634146800

p = 878502880971205563250537
a = 373244437522236692557633
b = 776663746981095848389651
c = 359706841382695634146800
# cy^2=x^3+ax+b
# => * c^3
# (c^2y)^2 = (cx)^3 + (c^2a)(cx)+(c^3b)
new_a = c**2*a%p
new_b = c**3*b%p
P = Ps[-1]
E = EllipticCurve(GF(p),[new_a,new_b])
flag1 = (long_to_bytes(G[0]))
G = E.point((c*G[0],c**2*G[1]))
P = E.point((c*P[0],c**2*P[1]))

Go = Integer(G.order())
flag2 = Integer(G.discrete_log(P))
for _ in range(2):
    print(flag1+long_to_bytes(flag2+_*Go))
    print(flag2+_*Go)
```





