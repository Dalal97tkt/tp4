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
//a function to determin if the point is the one at infinity 
int at_infinity(struct point P)
 {
  if((mpz_cmp_ui(P.x,0)==0)&&(mpz_cmp_ui(P.y,0)==0))
   {
    return 1;
   }
  return 0;
 } 
//the next nightmare
int doublement(struct point *R,struct point P, mpz_t p, mpz_t a, mpz_t b)
 {
  int res1=sur_courbe( P, p,a, b);
  if((res1==0))
   {
    return -1;
   }
   else if(!at_infinity(P))
    {
     struct point P_temp ;
     mpz_inits(P_temp.x,P_temp.y,NULL);
     mpz_set(P_temp.x,P.x);
     mpz_set(P_temp.y,P.y);
        mpz_t lamda;
        mpz_init(lamda);

        // lamda = (3xP2 + a) / 2yP
        mpz_powm_ui(R->x, P_temp.x, 2, p);
        mpz_mul_ui(R->x, R->x, 3);
        mpz_add(R->x, R->x, a);
        mpz_mod(R->x, R->x, p);

        mpz_mul_ui(lamda, P_temp.y, 2);
        mpz_invert(lamda, lamda, p);
        mpz_mul(lamda, R->x, lamda);
        mpz_mod(lamda, lamda, p);

        //xR = lamda2 - 2xP
        mpz_powm_ui(R->x, lamda, 2, p);
        mpz_submul_ui(R->x, P_temp.x, 2);
        mpz_mod(R->x, R->x, p);

        //yR = lamda * (xP - xR) - yP
        mpz_sub(R->y, P_temp.x, R->x);
        mpz_mul(R->y, lamda, R->y);
        mpz_sub(R->y, R->y, P_temp.y);
        mpz_mod(R->y, R->y, p);

        mpz_clears(lamda,P_temp.x,P_temp.y,NULL);
        return 0;
     }
    else
     {
      mpz_set(R->x,P.x);
      mpz_set(R->y,P.y);
      return 0;
     }
     
     
 }
     
 
//verifying if the function works fine
int main()
{
 //attempting to declare the struct
 struct point g;
 struct point *R;
 struct point r;
 R=&r;
 mpz_t x;
 mpz_t y;
 mpz_inits(x,y,NULL);
 mpz_init(g.x);
 mpz_init(g.y);
 mpz_inits(r.x,r.y,NULL);
 mpz_set(R->x,r.x);
 mpz_set(R->y,r.y);
 mpz_set_str(g.x,"188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012",16);
 mpz_set_str(g.y,"07192b95ffc8da78631011ed6b24cdd573f977a11e794811",16);
 mpz_t p,b,a;
 mpz_inits(p,b,a,NULL);
 mpz_set_str (p,"6277101735386680763835789423207666416083908700390324961279", 10 );
 mpz_set_str(b,"64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1",16);
 mpz_set_si(a,-3);
 int res=doublement(R,g,p,a,b);
 switch(res)
  {
   case -1:
   printf("One of the points is not on the curve an addition cannot be done\n");
   break;
   case 0:
   printf("the points are on the curve, additions can done, the resulting point is\n");
   gmp_printf("The coordinates of the point obtained by the addition are:\n {x:=%Zd}\n {y:=%Zd}\n",R->x,R->y);
  }
 mpz_clears(g.x,g.y,p,b,x,y,a,r.x,r.y,NULL);
 
 return 0;
}
