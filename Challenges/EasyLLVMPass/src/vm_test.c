#include <stdio.h>
#include <stdint.h>

unsigned char mem[] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0xff, 0,
//flag
81, 119, 81, 95, 115, 48, 111, 79, 111, 95, 115, 105, 109, 112, 108, 101, 95, 76, 108, 86, 109, 95, 80, 52, 115, 53
};

unsigned char opcode[26][16] = {
"aaLLzLzzJJaJLJa",
"bbJJyJyyMMbMJMb",
"ccRRxRxxNNcNRNc",
"ddJJwJwwRRdRJRd",
"eeSSvSvvKKeKSKe",
"ffVVuVuuPPfPVPf",
"ggMMtMttVVgVMVg",
"hhFFsFssRRhRFRh",
"iiIIrIrrJJiJIJi",
"jjGGqGqqZZjZGZj",
"kkVVpVppBBkBVBk",
"llEEoEooKKlKEKl",
"mmOOnOnnMMmMOMm",
"nnJJ_J__PPnPJPn",
"ooUU_U__MMoMUMo",
"ppUU_U__UUpUUUp",
"qqGG_G__OOqOGOq",
"rrVV_V__ZZrZVZr",
"ssWW_W__WWsWWWs",
"ttTT_T__WWtWTWt",
"uuRR_R__QQuQRQu",
"vvGG_G__RRvRGRv",
"wwOO_O__TTwTOTw",
"xxFF_F__VVxVFVx",
"yyKK_K__NNyNKNy",
"zzRR_R__CCzCRCz"
};

void vm()
{
    unsigned char *p_mem = mem;
    unsigned char PC, a, b, f, r;
    mem[0] = 1;
    for(int i = 0; i < 5; i++) {
        PC = *p_mem;
        a = p_mem[PC + 1];
        b = p_mem[PC];
        r = p_mem[PC + 2];
		*p_mem = PC + 3;
        f = ~(p_mem[b] & p_mem[a]);
        p_mem[r] = f;
    }
}
//miniLctf{QwQ_s0oOo_simple_LlVm_P4s5}

int main() {

    for(int i = 0; i < 26; i++)
    {
        for(int j = 0; j<15; j++)
        {
            mem[j+1]=opcode[i][j];
        }
        vm();
    }


    for(int i = 0; i < 26; i++)
    {
        printf("%d, ", mem[97+i]);
    }
    //

    return 0;
}