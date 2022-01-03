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
 
//the addition function
int addition(struct point *R,struct point P,struct  point Q, mpz_t p, mpz_t a, mpz_t b)
 {
  int res1=sur_courbe( P, p,a, b),res2=sur_courbe(Q,p,a,b);
  if((at_infinity(P)==0)&&(res2!=0))
   {
    mpz_set(R->x,Q.x);
    mpz_set(R->y,Q.y);
    printf("did this function work\n");
    return 0;
   }
  else if((at_infinity(Q)==0)&&(res1!=0))
    {
     mpz_set(R->x,Q.x);
     mpz_set(R->y,Q.y);
     return 0;
    }
  else if((res1==0)||(res2==0))
   {
    return -1;
   }
   else
    {
     mpz_t lambda,sum1,sum2,sum3;
     mpz_inits(lambda,sum1,sum2,sum3,NULL);
     mpz_sub(sum1,Q.y,P.y);//sum1=yq-yp
     mpz_sub(sum2,Q.x,P.x);//sum2=xq-xp
  
     mpz_invert(sum2,sum2,p);//sum2=(sum2)^-1 mod p
     mpz_mul(lambda,sum1,sum2);//lambda=(yq-yp)/(xq-xp)
     mpz_mod(lambda,lambda,p);//lambda=(sum1)/sum2 mod p
     mpz_pow_ui (sum1, lambda, 2);//sum1=(lambda)^2
     mpz_sub(sum1,sum1,P.x);//sum1=sum1-xp
     mpz_sub(sum1,sum1,Q.x);//sum1=sum1-xq
     mpz_mod(R->x,sum1,p);//R.x=(lambda^2)-xp-xq mod p
     mpz_sub(sum1,P.x,sum1);//sum1=xp-xr
     mpz_mul(sum1,lambda,sum1);//sum1=lambda*sum1
     mpz_sub(sum1,sum1,P.y);//R.y=(lambda)*(xp-xr)-yp
     mpz_mod(R->x,sum1,p);
     return 0;
     
    }
   
   
 }

//the doublement function
int doublement(struct point *R,struct point P, mpz_t p, mpz_t a, mpz_t b)
 {
  int res1=sur_courbe( P, p,a, b);
  if((res1==0))
   {
    return -1;
   }
   else if(!at_infinity(P))
    {
     mpz_t lambda, sum1,sum2;//defining lambda, and variables to contain some content to be able to do some operations
     mpz_inits(lambda,sum1,sum2,NULL);//initializing the variables
     mpz_powm_ui(sum1,P.x,2,p);//sum1=xp^2
     mpz_mul_ui(sum1,sum1,3);//sum1=3*sum1=3*xp^2
     mpz_add(sum1,sum1,a);//sum1=sum1+a=3*xp^2+a
     mpz_mod(sum1,sum1,p);//sum1=sum1 mod p
     mpz_mul_ui(sum2,P.y,2);//sum2=2*y
     mpz_invert (sum2,sum2,p );//sum2=(sum2)^-1 mod p
     mpz_mul(lambda,sum2,sum1);//lambda=(sum1)/sum1 
     mpz_mod(lambda,lambda,p);//lambda= lambda mod p=(3*xp^2+a)/(2*yp) mod p
     mpz_pow_ui(sum1,lambda,2);//sum1=(lambda)^2
     mpz_mul_ui(sum2,P.x,2);//sum2=2xp
     mpz_sub(sum1,sum1,sum2);//sum1=sum1-sum2
     mpz_mod(sum1,sum1,p);//sum1=sum1 mod p
     mpz_set(R->x,sum1);//xR=sum1+sum2=(lambda^2)-2xp mod p
     mpz_sub(sum1,P.x,sum1);//sum1=xp-xr
     mpz_mul(sum1,sum1,lambda);//sum1=sum1*lambda=(xp-xr)*lambda
     mpz_sub(sum1,sum1,P.y);//sum1= sum1 -yp
     mpz_mod(sum1,sum1,p);//sum1 =sum1 mod p
     mpz_set(R->y,sum1);//yr=sum1-yp=(lambda*(xp-xr))-yp mod p
     mpz_clears(lambda,sum1,sum2,NULL);
     return 0;
     }
    else
     {
      mpz_set(R->x,P.x);
      mpz_set(R->y,P.y);
      return 0;
     }
   }

//the function that will do k.P
int multiple(struct point *R, struct point P,mpz_t k, mpz_t p, mpz_t a, mpz_t b,struct point r)
 {
  struct point R_sub;//a created a case for the point R, it will contain the old value so that we cam use it again
  if (!sur_courbe( P,p,a,b))
   {
    return -1;
   }
  else if(at_infinity(P))
   {
    mpz_set(R->x,P.x);
    mpz_set(R->y,P.y);
    return 0;
   }
   else
    {
     int sz=mpz_sizeinbase(k,2),res=0;//
     for(int i=sz-1;i>=0;i--)
      {
      printf("%d\n",mpz_tstbit(k,i));
      printf("res of dob=%d\n",doublement(R, r, p,a,b));
       if(mpz_tstbit (k,i))
         {
          addition(R,r,P,p,a,b);
          printf("res of add=%d\n",addition(R,r,P,p,a,b));
         }
      }
      
     
     }
    return 0;
   }
   
//the main function 
int main(int argc, char* argv)
 {
  struct point g;
  struct point r;
  struct point *R;
  R=&r;
  
  mpz_inits(r.x,r.y,g.x,g.y,NULL);
  //an example
  mpz_set_str(g.x,"188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012",16);
  mpz_set_str(g.y,"07192b95ffc8da78631011ed6b24cdd573f977a11e794811",16);
  
  //the curve parameters
  mpz_t p,b,a;
  mpz_inits(p,b,a,NULL);
  mpz_set_str (p,"6277101735386680763835789423207666416083908700390324961279", 10 );
  mpz_set_str(b,"64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1",16);
  mpz_set_si(a,-3);
  
  //an example 
  mpz_t k;
  mpz_init(k);
  mpz_set_ui(k,2);
  int res=multiple(R,g,k,p,a,b,r);
  mpz_clear(k);
  switch(res)
   {
    case -1:
    printf("One of the points is not on the curve an addition cannot be done\n");
    break;
    case 0:
    printf("the points are on the curve, additions can done, the resulting point is\n");
    gmp_printf("The coordinates of the point obtained by the addition are:\n {x:=%Zd}\n {y:=%Zd}\n",R->x,R->y);
   }
  mpz_clears(g.x,g.y,p,b,a,r.x,r.y,NULL);
 
  return 0;
}
  
