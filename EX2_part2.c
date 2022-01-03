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
  mpz_sub(sum3,b,sum3);//sum3= b-3x
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
  if((res1==0)||(res2==0))
   {
    return -1;
   }
   else if((at_infinity(P)==0) &&(at_infinity(Q)==0)) 
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
   else if(at_infinity(P)==0)
    {
     mpz_set(R->x,Q.x);
     mpz_set(R->y,Q.y);
     return 0;
    }
   else
    {
     mpz_set(R->x,P.x);
     mpz_set(R->y,P.y);
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
     mpz_pow_ui(sum1,P.x,2);//sum1=xp^2
     mpz_mul_ui(sum1,sum1,3);//sum1=3*sum1=3*xp^2
     mpz_add(sum1,sum1,a);//sum1=sum1+a=3*xp^2+a
     mpz_mul_ui(sum2,P.y,2);//sum2=2*yp
     mpz_invert (sum2,sum2,p );//sum2=(sum2)^-1 mod p
     mpz_mul(lambda,sum2,sum1);//lambda=(sum1)/sum2 
     mpz_mod(lambda,lambda,p);//lambda= lambda mod p=(3*xp^2+a)/(2*yp) mod p
     mpz_pow_ui(sum1,lambda,2);//sum1=(lambda)^2
     mpz_mul_ui(sum2,P.x,2);//sum2=2xp
     mpz_sub(sum1,sum1,sum2);//sum1=sum1-sum2
     mpz_mod(sum1,sum1,p);//sum1=sum1 mod p
     mpz_set(R->x,sum1);//xR=sum1-sum2=(lambda^2)-2xp mod p
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
      printf("the point is at infinity?\n");
      mpz_set(R->x,P.x);
      mpz_set(R->y,P.y);
      return 0;
     }
   }

int multiple(struct point *R, struct point P,mpz_t k, mpz_t p, mpz_t a, mpz_t b,struct point r)
 {
 // struct point R_sub;//a created a case for the point R, it will contain the old value so that we cam use it again
  //mpz_inits(R_sub.x,R_sub.y,NULL);
  mpz_set(r.x,P.x);//we set the first value to P
  mpz_set(r.y,P.y);
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
     gmp_printf("the size in base 2 of %Zd is %d\n",k,sz);
     for(int i=sz;i>0;i--)
      {
       doublement(R, r, p,a,b);
       gmp_printf("
       printf("is the loop working%d\n",i);
       printf("%d\n",mpz_tstbit(k,i));
       if(mpz_tstbit (k,i))
         {
          
          addition(R,r,P,p,a,b);
          //mpz_set(R_sub.x,R->x);
          //mpz_set(R_sub.y,R->y);
         }
      }
     return 0;
     }
     
     //mpz_clears(R_sub.x,R_sub.y,NULL);
   }
   
//the main function 
int main(int argc, char* argv[])
 {
  if(argc!=2)
   {
    printf("using %s: k\n",argv[0]);
    exit(-1);
   }
  //taking k as input
  int k=atoi(argv[1]);
  
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
  //mpz_set_str (p,"6277101735386680763835789423207666416083908700390324961279", 10 );
  //mpz_set_str(b,"64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1",16);
 // mpz_set_ui(a,3);
  
  //reading input from a file 
  FILE *fp;
  fp=fopen("parameters.txt","r");
  gmp_fscanf(fp,"%*s%*s%Zd%*s%*s%Zx%*s%*s%Zd",a,b,p);
  gmp_printf("a=%Zd\nb=%Zx\np=%Zd\n",a,b,p);
  fclose(fp);
  //an example 
  mpz_t z_k,z_k1;
  mpz_inits(z_k,z_k1,NULL);
  mpz_set_ui(z_k,k);
  printf("First we start with the point G that we know is on the curve\n");
  gmp_printf("The coordinates of G are:\n {x:=%Zd}\n {y:=%Zd}\n",g.x,g.y);
  int res;
  printf("we are testing for k =12, a version of doublement, we start with 2.(k/2)\n");
  if(mpz_even_p(z_k))
   {
    //printf("the first k.P is equal to:\n");
    mpz_cdiv_q_ui (z_k1, z_k,2 );//z_k1=z_k/2
    printf("now let's test the multiple function");
    res=multiple( R, g, z_k1, p, a, b, r);
    
    if(res==-1){printf("we did something wrong\n");}
    //gmp_printf("The coordinates of the point obtained by the addition are:\n {x:=%Zd}\n {y:=%Zd}\n",R->x,R->y);
    gmp_printf("The coordinates before the operation\n {x:=%Zd}\n {y:=%Zd}\n",r.x,r.y);
    gmp_printf("The coordinates of r are:\n {x:=%Zd}\n {y:=%Zd}\n",r.x,r.y);
    res=doublement( R, r, p, a, b);
    mpz_set(r.x,R->x);
    mpz_set(r.y,R->y);
    printf("the result of the operation is %d\n",res);
    gmp_printf("The coordinates after the operation are:\n {x:=%Zd}\n {y:=%Zd}\n",r.x,r.y);
    gmp_printf("The coordinates of the point obtained by the addition are:\n {x:=%Zd}\n {y:=%Zd}\n",R->x,R->y);
   
   }
    
    
  
  
  mpz_clears(z_k,z_k1,NULL);
 
  mpz_clears(g.x,g.y,p,b,a,r.x,r.y,NULL);
 
  return 0;
}
