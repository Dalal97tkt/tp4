#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>
#include <openssl/sha.h>
#include<string.h>



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
  mpz_t k1,k2;
  mpz_inits(k1,k2,NULL);
  unsigned long int K2;
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
  else if(mpz_even_p (k))
   {
    mpz_cdiv_q_ui (k2 ,k,2);
    K2=mpz_get_ui (k2);
    printf("are we even\n");
    for(int i=1;i<=K2;i++)
     {
      printf("%d\n",i);
      doublement(R,P,p,a,b);
     }
     return 0;
    }
   else 
    {
     mpz_sub_ui(k1,k,1);
     mpz_cdiv_q_ui (k2 ,k1,2);
     K2=mpz_get_ui (k2);
     printf("are we here odd\n");
     for(int i=1;i<=K2;i++)
     {
      doublement(R,P,p,a,b);
      printf("%d\n",i);
     }
     
     addition(R,P,r,p,a,b);
     return 0;
     }
     mpz_clears(k1,k2,NULL);
   }
   
//the main function 
int main(int argc, char* argv[])
 {
  if(argc!=2)
   {
    printf("using %s:message.frmt  \n",argv[0]);
    exit(-1);
   }
  
  struct point g;
  struct point r;
  struct point *R;
  struct point q;
  R=&r;
  
  mpz_inits(r.x,r.y,g.x,g.y,q.x,q.y,NULL);
  //an example
  mpz_set_str(g.x,"188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012",16);
  mpz_set_str(g.y,"07192b95ffc8da78631011ed6b24cdd573f977a11e794811",16);
  //the curve parameters
  mpz_t p,b,a,n,d,m,res,s,k;
  mpz_inits(p,b,a,n,d,m,res,s,k,NULL);
 
  //reading input from a file 
  FILE *fp;
  printf("%s\n",argv[1]);
  fp=fopen(argv[1],"r");
  gmp_fscanf(fp,"%*s%*s%Zd",m);
  gmp_printf("m=%Zd\n",m);
  char *buf[1025];
  mpz_get_str (buf ,10,m);
  printf("are we getting the correct string");
  //unsigned char ibuf[] = buf;//this string will contain the message
  fclose(fp);
  
  //opening the files containing the info
  FILE *fp1,*fp2;
  fp1=fopen("parameters.txt","r");//the file containing the parameters
  fp2=fopen("the_keys.txt","r");//the file containing the keys
  gmp_fscanf(fp1,"%*s%*s%Zd%*s%*s%Zx%*s%*s%Zd%*s%*s%Zd",a,b,p,n);
  gmp_printf("a=%Zd\nb=%Zx\np=%Zd\nn=%Zd\n",a,b,p,n);
  gmp_fscanf(fp2,"%*s%*s%Zx%*s%*s%Zx%*s%*s%Zx",d,q.x,q.y);
  printf("are we scanning good\n");
  gmp_printf("the private key is %Zd\n",d);
  mpz_set_ui(d,12345);//this is just a sample to make everything easier, the actual d is calculated above 
  //calculating the 256 hash
  unsigned char obuf[SHA256_DIGEST_LENGTH];
  SHA256(buf, strlen(buf), obuf);//buf contain the hash of m
  mpz_set_ui(n,1000);
  //declaring the generator
  gmp_randstate_t generator;
  gmp_randinit_default(generator); // Initializing the generator
  gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed
  do
   {
     mpz_urandomm (k,generator,n);
     multiple(R,g,k,p,a,b,r);
     mpz_mod(res,R->x,n);//res=xr mod n
     mpz_mul(res,res,d);
     mpz_add_ui(res,res,buf);
     mpz_invert(k,k,n);
     mpz_mul(s,res,k);
     mpz_mod(s,s,n);
    }while((mpz_cmp_ui(res,0)!=0)&&(mpz_cmp_ui(s,0)!=0));
    
   gmp_printf("the signature is s=%Zd\nr=%Zd\n",s,res);
  
  
  
  
  fclose(fp1);
  fclose(fp2);
  
  mpz_clears(g.x,g.y,p,b,a,r.x,r.y,n,d,m,q.x,q.y,res,s,k,NULL);
 
  return 0;
}
