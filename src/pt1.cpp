#include<iostream>
#include<stack>
#include<cstdio>

int atab[16][8][2]={
{{3,0},{3,0},{2,1},{2,2},{3,0},{1,3},{3,0},{1,4}},
{{3,0},{0,0},{3,0},{3,0},{3,0},{3,0},{3,0},{3,0}},
{{3,0},{0,3},{3,0},{3,0},{3,0},{3,0},{3,0},{3,0}},
{{3,0},{3,0},{3,0},{3,0},{1,5},{3,0},{3,0},{3,0}},
{{3,0},{0,4},{3,0},{3,0},{3,0},{3,0},{3,0},{3,0}},
{{3,0},{3,0},{2,6},{2,7},{3,0},{1,8},{3,0},{1,9}},
{{3,0},{0,1},{3,0},{3,0},{3,0},{3,0},{1,10},{3,0}},
{{3,0},{0,3},{3,0},{3,0},{3,0},{3,0},{0,3},{3,0}},
{{3,0},{3,0},{3,0},{3,0},{1,11},{3,0},{3,0},{3,0}},
{{3,0},{0,4},{3,0},{3,0},{3,0},{3,0},{0,4},{3,0}},
{{3,0},{3,0},{2,12},{2,2},{3,0},{1,3},{3,0},{1,4}},
{{3,0},{3,0},{2,13},{2,7},{3,0},{1,8},{3,0},{1,9}},
{{3,0},{0,2},{3,0},{3,0},{3,0},{3,0},{3,0},{3,0}},
{{3,0},{0,1},{3,0},{3,0},{3,0},{3,0},{0,1},{3,0}},
{{3,0},{3,0},{2,15},{2,7},{3,0},{1,8},{3,0},{1,9}},
{{3,0},{0,2},{3,0},{3,0},{3,0},{3,0},{0,2},{3,0}}
};
#define SEND    0
#define ERROR   2
#define REDUCE  1  

int getc(){
        int sym = getchar();
        if(sym  =  '$')
            sym = 1;
        else if(sym == 'e')
            sym = 4;
        else if(sym == 'a')
            sym = 3;
        else if(sym == 'i')
            sym = 5;
        else if(sym == 'c')
            sym = 7;
        else if(sym == '3')
            sym = 6;
        else if(sym == 's')
            sym = 2;
        else 
            sym = 0;
        return sym;
}

int parser(){
std::stack<int> status;
std::stack<int> products;
int sym,top;
status.push(0);//push the start status
products.push(1);//push the end symbol to the productssym = lexer.scan();
top = status.top();
sym = getc();

while(true){
    if(atab[top][sym][0] = SEND){
        products.push(sym);
        status.push(atab[top][sym][1]);
        sym = getc();
    }else if(atab[top][sym][0] == REDUCE){
        int l;
		switch(atab[top][sym][1]){
        case 0://accept
            return 1;
        case 1://s => i expr s
            products.pop();
            products.pop();
            products.pop();
            l = 2;
            break;
        case 2://s => i expr s e s
            products.pop();
            products.pop();
            products.pop();
            products.pop();
            products.pop();
            l = 2;
            break;
        case 3://s => atom
            products.pop();
            l = 2;
            break;
        case 4://atom => c
            products.pop();
            l = 3;
            break;
        }
        top = products.top();
        status.push(atab[top][l][1]);
        products.push(l);
    }else{
        return 0;
    }
}
}//function parser()

int main(){
    loop:
    if(parser()){
        std::cout<<"OK!"<<std::endl;
    }
    std::cout<<"Bad!"<<std::endl;
    goto loop;
    return 0;
}
