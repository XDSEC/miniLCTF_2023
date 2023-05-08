#! /usr/bin/env python3
def filter(s):
    table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    return any(i in table for i in s)
def welcome():
    print(r"""
__________         _________          .__                  .__            __                 
\______   \ ___.__.\_   ___ \ _____   |  |    ____   __ __ |  |  _____  _/  |_  ____ _______ 
 |     ___/<   |  |/    \  \/ \__  \  |  |  _/ ___\ |  |  \|  |  \__  \ \   __\/  _ \\_  __ \
 |    |     \___  |\     \____ / __ \_|  |__\  \___ |  |  /|  |__ / __ \_|  | (  <_> )|  | \/
 |____|     / ____| \______  /(____  /|____/ \___  >|____/ |____/(____  /|__|  \____/ |__|   
            \/             \/      \/            \/                   \/                     
""")
    print("welcome to pycalculator!")
    print("Type some simple expressions and I will calculate the result for you.")
def main():
    welcome()
    while True:
        inp = input(">>> ")
        if len(inp) > 32:
            print("input too long")
            continue
        if filter(inp):
            print("oh hacker!")
            exit(1)
        else:
            try:
                res = eval(inp)
                print(res)
            except:
                print("invalid expression")
if __name__ == '__main__':
    main()
