# minilinux wp by dr3

使用make run运行busybox加qemu做的一个linux虚拟机

很显然是要分析ko模块，就是个内核，和linux本体没有太大的关系

ida打开发现加载了一个设备

很容易定位到关键函数sub_DE0

obfs_decode是一个加法加密，简单解密可以得到
2b7e151628aed2a6abf7158809cf4f3c

下面通过sprintf来将其转换为了内存中的值

下面标注后关键加密和check函数

```c
bool __fastcall sub_BF0(__int64 input_end, char *input_begin)
{
  _BOOL4 v2; // eax
  _BYTE ctx[176]; // [rsp-F0h] [rbp-F0h] BYREF
  __int128 v5; // [rsp-40h] [rbp-40h]
  unsigned __int64 v6; // [rsp-30h] [rbp-30h]
  unsigned __int64 v7; // [rsp-28h] [rbp-28h]
  unsigned __int64 v8; // [rsp-20h] [rbp-20h]

  _fentry__(input_end);
  v8 = __readgsqword(0x28u);
  *(_QWORD *)&v5 = 0x5A96B0813E935426LL;
  *((_QWORD *)&v5 + 1) = 0x86E97215021B2394LL;
  v6 = 0xCA67F9B2C8B5F4C9LL;
  v7 = 0xB3E603429B5AFA0ALL;
  memset(ctx, 0, sizeof(ctx));
  aes_init(ctx, input_begin);
  aes_enc((_BYTE *)input_end, (__int64)ctx);
  aes_enc((_BYTE *)(input_end + 16), (__int64)ctx);
  v2 = *(_OWORD *)input_end != v5 || *(_QWORD *)(input_end + 16) != v6 || *(_QWORD *)(input_end + 24) != v7;
  return !v2;
}
```

是个魔改的aes，具体可以findcrypt查询魔数

魔改的点在

```C
v9 = *(_BYTE *)(v2 + v6) ^ v3[v6];  
```

就是add_round_key这里魔改了，其实很简单，
改一下aes解密的源码就可以，同样进行一下异或就行

```C
(*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
(*state)[i][j] ^= 0x42;
```

解密即可