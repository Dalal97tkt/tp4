#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>

//defining the struct point
struct point{
mpz_t x;
mpz_t y;
} ;

//a function to determine if P is a point on the curve 
int sur_courbe( struct point g, mpz_t p,mpz_t a, mpz_t b)
 {
  mpz_t sum1,sum2,sum3;
  mpz_inits(sum1,sum2,sum3,NULL);
  mpz_powm_ui(sum1,g.y,2,p);//sum1=y^2 mod p
  mpz_pow_ui(sum2,g.x,3);//sum2=x^3 
  mpz_mul(sum3,g.x,a);//sum3= 3x 
  mpz_add(sum3,b,sum3);//sum3= b-3x
  mpz_add(sum2,sum2,sum3);//sum2=sum2+sum3 = x^3-3x+b
  mpz_mod (sum2,sum2,p);//sum2=sum2 mod p
  if(mpz_cmp(sum1,sum2)==0)
   {
    return 1;
   }
   return 0;
 }
 
 
//verifying if the function works fine
int main()
{
 //attempting to declare the struct
 struct point g;
 mpz_t x;
 mpz_t y;
 mpz_inits(x,y,NULL);
 mpz_init(g.x);
 mpz_init(g.y);
 
 mpz_set_str(g.x,"188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012",16);
 mpz_set_str(g.y,"07192b95ffc8da78631011ed6b24cdd573f977a11e794811",16);
 mpz_t p,b,a;
 mpz_inits(p,b,a,NULL);
 mpz_set_str (p,"6277101735386680763835789423207666416083908700390324961279", 10 );
 mpz_set_str(b,"64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1",16);
 mpz_set_si(a,-3);
 int res=sur_courbe(g, p,a, b);
 switch(res)
  {
   case 0:
   printf("the point is not on the curve\n");
   break;
   case 1:
   printf("the point is on the curve\n");
  }
 mpz_clears(g.x,g.y,p,b,x,y,a,NULL);
 
 return 0;
}
 
 
 
   
