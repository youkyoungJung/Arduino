/* KISA ARIA-32bit Implementation for Arduino Platform */
#include <stdio.h>
#define   Byte    unsigned char
#define   Word    unsigned long

#define   Key_Size 128    // # of rounds for 256 : 16, for 192: 14, for 128: 12
#define   LITTLE_ENDIAN

const Word KRK[3][4] = {
  {0x517cc1b7, 0x27220a94, 0xfe13abe8, 0xfa9a6ee0},
  {0x6db14acc, 0x9e21c820, 0xff28b1d5, 0xef5de2b0},
  {0xdb92371d, 0x2126e970, 0x03249775, 0x04e8c90e} };

/* S-box들을 정의하기 위한 마크로. */
#define AAA(V) 0x ## 00 ## V ## V ## V
#define BBB(V) 0x ## V ## 00 ## V ## V
#define CCC(V) 0x ## V ## V ## 00 ## V
#define DDD(V) 0x ## V ## V ## V ## 00
#define XX(NNN,x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,xa,xb,xc,xd,xe,xf)		\
  NNN(x0),NNN(x1),NNN(x2),NNN(x3),NNN(x4),NNN(x5),NNN(x6),NNN(x7),	\
    NNN(x8),NNN(x9),NNN(xa),NNN(xb),NNN(xc),NNN(xd),NNN(xe),NNN(xf)

const Word S1[256]={
  XX(AAA,63,7c,77,7b,f2,6b,6f,c5,30,01,67,2b,fe,d7,ab,76),
  XX(AAA,ca,82,c9,7d,fa,59,47,f0,ad,d4,a2,af,9c,a4,72,c0),
  XX(AAA,b7,fd,93,26,36,3f,f7,cc,34,a5,e5,f1,71,d8,31,15),
  XX(AAA,04,c7,23,c3,18,96,05,9a,07,12,80,e2,eb,27,b2,75),
  XX(AAA,09,83,2c,1a,1b,6e,5a,a0,52,3b,d6,b3,29,e3,2f,84),
  XX(AAA,53,d1,00,ed,20,fc,b1,5b,6a,cb,be,39,4a,4c,58,cf),
  XX(AAA,d0,ef,aa,fb,43,4d,33,85,45,f9,02,7f,50,3c,9f,a8),
  XX(AAA,51,a3,40,8f,92,9d,38,f5,bc,b6,da,21,10,ff,f3,d2),
  XX(AAA,cd,0c,13,ec,5f,97,44,17,c4,a7,7e,3d,64,5d,19,73),
  XX(AAA,60,81,4f,dc,22,2a,90,88,46,ee,b8,14,de,5e,0b,db),
  XX(AAA,e0,32,3a,0a,49,06,24,5c,c2,d3,ac,62,91,95,e4,79),
  XX(AAA,e7,c8,37,6d,8d,d5,4e,a9,6c,56,f4,ea,65,7a,ae,08),
  XX(AAA,ba,78,25,2e,1c,a6,b4,c6,e8,dd,74,1f,4b,bd,8b,8a),
  XX(AAA,70,3e,b5,66,48,03,f6,0e,61,35,57,b9,86,c1,1d,9e),
  XX(AAA,e1,f8,98,11,69,d9,8e,94,9b,1e,87,e9,ce,55,28,df),
  XX(AAA,8c,a1,89,0d,bf,e6,42,68,41,99,2d,0f,b0,54,bb,16)
};

const Word S2[256]={
  XX(BBB,e2,4e,54,fc,94,c2,4a,cc,62,0d,6a,46,3c,4d,8b,d1),
  XX(BBB,5e,fa,64,cb,b4,97,be,2b,bc,77,2e,03,d3,19,59,c1),
  XX(BBB,1d,06,41,6b,55,f0,99,69,ea,9c,18,ae,63,df,e7,bb),
  XX(BBB,00,73,66,fb,96,4c,85,e4,3a,09,45,aa,0f,ee,10,eb),
  XX(BBB,2d,7f,f4,29,ac,cf,ad,91,8d,78,c8,95,f9,2f,ce,cd),
  XX(BBB,08,7a,88,38,5c,83,2a,28,47,db,b8,c7,93,a4,12,53),
  XX(BBB,ff,87,0e,31,36,21,58,48,01,8e,37,74,32,ca,e9,b1),
  XX(BBB,b7,ab,0c,d7,c4,56,42,26,07,98,60,d9,b6,b9,11,40),
  XX(BBB,ec,20,8c,bd,a0,c9,84,04,49,23,f1,4f,50,1f,13,dc),
  XX(BBB,d8,c0,9e,57,e3,c3,7b,65,3b,02,8f,3e,e8,25,92,e5),
  XX(BBB,15,dd,fd,17,a9,bf,d4,9a,7e,c5,39,67,fe,76,9d,43),
  XX(BBB,a7,e1,d0,f5,68,f2,1b,34,70,05,a3,8a,d5,79,86,a8),
  XX(BBB,30,c6,51,4b,1e,a6,27,f6,35,d2,6e,24,16,82,5f,da),
  XX(BBB,e6,75,a2,ef,2c,b2,1c,9f,5d,6f,80,0a,72,44,9b,6c),
  XX(BBB,90,0b,5b,33,7d,5a,52,f3,61,a1,f7,b0,d6,3f,7c,6d),
  XX(BBB,ed,14,e0,a5,3d,22,b3,f8,89,de,71,1a,af,ba,b5,81)
};

const Word X1[256]={
  XX(CCC,52,09,6a,d5,30,36,a5,38,bf,40,a3,9e,81,f3,d7,fb),
  XX(CCC,7c,e3,39,82,9b,2f,ff,87,34,8e,43,44,c4,de,e9,cb),
  XX(CCC,54,7b,94,32,a6,c2,23,3d,ee,4c,95,0b,42,fa,c3,4e),
  XX(CCC,08,2e,a1,66,28,d9,24,b2,76,5b,a2,49,6d,8b,d1,25),
  XX(CCC,72,f8,f6,64,86,68,98,16,d4,a4,5c,cc,5d,65,b6,92),
  XX(CCC,6c,70,48,50,fd,ed,b9,da,5e,15,46,57,a7,8d,9d,84),
  XX(CCC,90,d8,ab,00,8c,bc,d3,0a,f7,e4,58,05,b8,b3,45,06),
  XX(CCC,d0,2c,1e,8f,ca,3f,0f,02,c1,af,bd,03,01,13,8a,6b),
  XX(CCC,3a,91,11,41,4f,67,dc,ea,97,f2,cf,ce,f0,b4,e6,73),
  XX(CCC,96,ac,74,22,e7,ad,35,85,e2,f9,37,e8,1c,75,df,6e),
  XX(CCC,47,f1,1a,71,1d,29,c5,89,6f,b7,62,0e,aa,18,be,1b),
  XX(CCC,fc,56,3e,4b,c6,d2,79,20,9a,db,c0,fe,78,cd,5a,f4),
  XX(CCC,1f,dd,a8,33,88,07,c7,31,b1,12,10,59,27,80,ec,5f),
  XX(CCC,60,51,7f,a9,19,b5,4a,0d,2d,e5,7a,9f,93,c9,9c,ef),
  XX(CCC,a0,e0,3b,4d,ae,2a,f5,b0,c8,eb,bb,3c,83,53,99,61),
  XX(CCC,17,2b,04,7e,ba,77,d6,26,e1,69,14,63,55,21,0c,7d)
};

const Word X2[256]={
  XX(DDD,30,68,99,1b,87,b9,21,78,50,39,db,e1,72,09,62,3c),
  XX(DDD,3e,7e,5e,8e,f1,a0,cc,a3,2a,1d,fb,b6,d6,20,c4,8d),
  XX(DDD,81,65,f5,89,cb,9d,77,c6,57,43,56,17,d4,40,1a,4d),
  XX(DDD,c0,63,6c,e3,b7,c8,64,6a,53,aa,38,98,0c,f4,9b,ed),
  XX(DDD,7f,22,76,af,dd,3a,0b,58,67,88,06,c3,35,0d,01,8b),
  XX(DDD,8c,c2,e6,5f,02,24,75,93,66,1e,e5,e2,54,d8,10,ce),
  XX(DDD,7a,e8,08,2c,12,97,32,ab,b4,27,0a,23,df,ef,ca,d9),
  XX(DDD,b8,fa,dc,31,6b,d1,ad,19,49,bd,51,96,ee,e4,a8,41),
  XX(DDD,da,ff,cd,55,86,36,be,61,52,f8,bb,0e,82,48,69,9a),
  XX(DDD,e0,47,9e,5c,04,4b,34,15,79,26,a7,de,29,ae,92,d7),
  XX(DDD,84,e9,d2,ba,5d,f3,c5,b0,bf,a4,3b,71,44,46,2b,fc),
  XX(DDD,eb,6f,d5,f6,14,fe,7c,70,5a,7d,fd,2f,18,83,16,a5),
  XX(DDD,91,1f,05,95,74,a9,c1,5b,4a,85,6d,13,07,4f,4e,45),
  XX(DDD,b2,0f,c9,1c,a6,bc,ec,73,90,7b,cf,59,8f,a1,f9,2d),
  XX(DDD,f2,b1,00,94,37,9f,d0,2e,9c,6e,28,3f,80,f0,3d,d3),
  XX(DDD,25,8a,b5,e7,42,b3,c7,ea,f7,4c,11,33,03,a2,ac,60)
};

/* BY(X, Y)는 Word X의 Y번째 바이트, BRF(T,R)은 T>>R의 하위 1바이트
 * WO(X, Y)는 Byte array X를 Word array로 간주할 때 Y번째 Word  */
#define BY(X,Y) (((Byte *)(&X))[Y])
#define BRF(T,R) ((Byte)((T)>>(R)))
#define WO(X,Y) (((Word *)(X))[Y])

#define ReverseWord(W) { (W)=(W)<<24 ^ (W)>>24 ^ ((W)&0x0000ff00)<<8 ^ ((W)&0x00ff0000)>>8;  }

/* Byte array를 Word에 싣는 함수.  LITTLE_ENDIAN의 경우  엔디안 변환 과정을 거친다. */
#ifdef LITTLE_ENDIAN
#define WordLoad(ORIG, DEST) {			\
    Word ___t;					\
    BY(___t,0)=BY(ORIG,3);			\
    BY(___t,1)=BY(ORIG,2);			\
    BY(___t,2)=BY(ORIG,1);			\
    BY(___t,3)=BY(ORIG,0);			\
    DEST=___t;					\
  }
#else
#define WordLoad(ORIG, DEST) {			\
    DEST = ORIG;				\
  }
#endif

/* Key XOR Layer */
#define KXL { t0^=WO(rk,0); t1^=WO(rk,1); t2^=WO(rk,2); t3^=WO(rk,3);  rk += 16;  }

/* S-Box Layer 1 + M 변환 */
#define SBL1_M(T0,T1,T2,T3) {						\
    T0=S1[BRF(T0,24)]^S2[BRF(T0,16)]^X1[BRF(T0,8)]^X2[BRF(T0,0)];	\
    T1=S1[BRF(T1,24)]^S2[BRF(T1,16)]^X1[BRF(T1,8)]^X2[BRF(T1,0)];	\
    T2=S1[BRF(T2,24)]^S2[BRF(T2,16)]^X1[BRF(T2,8)]^X2[BRF(T2,0)];	\
    T3=S1[BRF(T3,24)]^S2[BRF(T3,16)]^X1[BRF(T3,8)]^X2[BRF(T3,0)];	\
  }
/* S-Box Layer 2 + M 변환 */
#define SBL2_M(T0,T1,T2,T3) {						\
    T0=X1[BRF(T0,24)]^X2[BRF(T0,16)]^S1[BRF(T0,8)]^S2[BRF(T0,0)];	\
    T1=X1[BRF(T1,24)]^X2[BRF(T1,16)]^S1[BRF(T1,8)]^S2[BRF(T1,0)];	\
    T2=X1[BRF(T2,24)]^X2[BRF(T2,16)]^S1[BRF(T2,8)]^S2[BRF(T2,0)];	\
    T3=X1[BRF(T3,24)]^X2[BRF(T3,16)]^S1[BRF(T3,8)]^S2[BRF(T3,0)];	\
  }
/* 워드 단위의 변환 */
#define MM(T0,T1,T2,T3) {			\
    (T1)^=(T2); (T2)^=(T3); (T0)^=(T1);		\
    (T3)^=(T1); (T2)^=(T0); (T1)^=(T2);		\
  }
/* P 변환.  확산 계층의 중간에 들어가는 바이트 단위 변환이다. 이 부분은 endian과 무관하다.  */
#define P(T0,T1,T2,T3) {					\
    (T1) = (((T1)<< 8)&0xff00ff00) ^ (((T1)>> 8)&0x00ff00ff);	\
    (T2) = (((T2)<<16)&0xffff0000) ^ (((T2)>>16)&0x0000ffff);	\
    ReverseWord((T3));						\
  }

 /* FO: 홀수번째 라운드의 F 함수
  * FE: 짝수번째 라운드의 F 함수
  * MM과 P는 바이트 단위에서 endian에 무관하게 동일한 결과를 주며, 또한 endian 변환과 가환이다.  
  * 또한, SBLi_M은 LITTLE_ENDIAN에서 결과적으로 Word 단위로 endian을 뒤집은 결과를 준다.
  * 즉, FO, FE는 BIG_ENDIAN 환경에서는 ARIA spec과 동일한 결과를,
  * LITTLE_ENDIAN 환경에서는 ARIA spec에서 정의한 변환+endian 변환을 준다. */
#define FO {SBL1_M(t0,t1,t2,t3) MM(t0,t1,t2,t3) P(t0,t1,t2,t3) MM(t0,t1,t2,t3)}
#define FE {SBL2_M(t0,t1,t2,t3) MM(t0,t1,t2,t3) P(t2,t3,t0,t1) MM(t0,t1,t2,t3)}

/* n-bit right shift of Y XORed to X */
/* Word 단위로 정의된 블록에서의 회전 + XOR이다. */
#define GSRK(X, Y, n) {							\
    q = 4-((n)/32);							\
    r = (n) % 32;							\
    WO(rk,0) = ((X)[0]) ^ (((Y)[(q  )%4])>>r) ^ (((Y)[(q+3)%4])<<(32-r)); \
    WO(rk,1) = ((X)[1]) ^ (((Y)[(q+1)%4])>>r) ^ (((Y)[(q  )%4])<<(32-r)); \
    WO(rk,2) = ((X)[2]) ^ (((Y)[(q+2)%4])>>r) ^ (((Y)[(q+1)%4])<<(32-r)); \
    WO(rk,3) = ((X)[3]) ^ (((Y)[(q+3)%4])>>r) ^ (((Y)[(q+2)%4])<<(32-r)); \
    rk += 16;								\
  }

/* DecKeySetup()에서 사용하는 마크로 */
#define WordM1(X,Y) {  Y=(X)<<8 ^ (X)>>8 ^ (X)<<16 ^ (X)>>16 ^ (X)<<24 ^ (X)>>24;  }


/*************** Function *************************************************/
/* 암호화 함수. const Byte *i: 입력, int Nr: 라운드 수, const Byte *rk: 라운드 키들, Byte *o: 출력 */
void Crypt(const Byte *i, int Nr, const Byte *rk, Byte *o) {
  register Word t0, t1, t2, t3;
  
  WordLoad(WO(i,0), t0); WordLoad(WO(i,1), t1);
  WordLoad(WO(i,2), t2); WordLoad(WO(i,3), t3);
  
  if (Nr > 12) {KXL FO KXL FE}
  if (Nr > 14) {KXL FO KXL FE}
  KXL FO KXL FE KXL FO KXL FE KXL FO KXL FE 
  KXL FO KXL FE KXL FO KXL FE KXL FO KXL

  /* 최종 라운드는 특별함 */
#ifdef LITTLE_ENDIAN
  o[ 0] = (Byte)(X1[BRF(t0,24)]   ) ^ rk[ 3];
  o[ 1] = (Byte)(X2[BRF(t0,16)]>>8) ^ rk[ 2];
  o[ 2] = (Byte)(S1[BRF(t0, 8)]   ) ^ rk[ 1];
  o[ 3] = (Byte)(S2[BRF(t0, 0)]   ) ^ rk[ 0];
  o[ 4] = (Byte)(X1[BRF(t1,24)]   ) ^ rk[ 7];
  o[ 5] = (Byte)(X2[BRF(t1,16)]>>8) ^ rk[ 6];
  o[ 6] = (Byte)(S1[BRF(t1, 8)]   ) ^ rk[ 5];
  o[ 7] = (Byte)(S2[BRF(t1, 0)]   ) ^ rk[ 4];
  o[ 8] = (Byte)(X1[BRF(t2,24)]   ) ^ rk[11];
  o[ 9] = (Byte)(X2[BRF(t2,16)]>>8) ^ rk[10];
  o[10] = (Byte)(S1[BRF(t2, 8)]   ) ^ rk[ 9];
  o[11] = (Byte)(S2[BRF(t2, 0)]   ) ^ rk[ 8];
  o[12] = (Byte)(X1[BRF(t3,24)]   ) ^ rk[15];
  o[13] = (Byte)(X2[BRF(t3,16)]>>8) ^ rk[14];
  o[14] = (Byte)(S1[BRF(t3, 8)]   ) ^ rk[13];
  o[15] = (Byte)(S2[BRF(t3, 0)]   ) ^ rk[12];
#else
  o[ 0] = (Byte)(X1[BRF(t0,24)]   );
  o[ 1] = (Byte)(X2[BRF(t0,16)]>>8);
  o[ 2] = (Byte)(S1[BRF(t0, 8)]   );
  o[ 3] = (Byte)(S2[BRF(t0, 0)]   );
  o[ 4] = (Byte)(X1[BRF(t1,24)]   );
  o[ 5] = (Byte)(X2[BRF(t1,16)]>>8);
  o[ 6] = (Byte)(S1[BRF(t1, 8)]   );
  o[ 7] = (Byte)(S2[BRF(t1, 0)]   );
  o[ 8] = (Byte)(X1[BRF(t2,24)]   );
  o[ 9] = (Byte)(X2[BRF(t2,16)]>>8);
  o[10] = (Byte)(S1[BRF(t2, 8)]   );
  o[11] = (Byte)(S2[BRF(t2, 0)]   );
  o[12] = (Byte)(X1[BRF(t3,24)]   );
  o[13] = (Byte)(X2[BRF(t3,16)]>>8);
  o[14] = (Byte)(S1[BRF(t3, 8)]   );
  o[15] = (Byte)(S2[BRF(t3, 0)]   );
  WO(o,0)^=WO(rk,0); WO(o,1)^=WO(rk,1);
  WO(o,2)^=WO(rk,2); WO(o,3)^=WO(rk,3);
#endif
}

/* 암호화 라운드 키 생성. const Byte *mk: 마스터 키, Byte *rk: 라운드 키, int keyBits: 마스터 키의 길이 */
int EncKeySetup(const Byte *mk, Byte *rk, int keyBits) {
  register Word t0, t1, t2, t3;
  Word w0[4], w1[4], w2[4], w3[4];
  int q, r, i;
  
  WordLoad(WO(mk,0), w0[0]); WordLoad(WO(mk,1), w0[1]);
  WordLoad(WO(mk,2), w0[2]); WordLoad(WO(mk,3), w0[3]);
    
  q = (keyBits - 128) / 64;
  t0=w0[0]^KRK[q][0]; t1=w0[1]^KRK[q][1];
  t2=w0[2]^KRK[q][2]; t3=w0[3]^KRK[q][3];   
  FO;
  
  if (keyBits > 128) {
    WordLoad(WO(mk,4), w1[0]);
    WordLoad(WO(mk,5), w1[1]);
    if (keyBits > 192) {
      WordLoad(WO(mk,6), w1[2]);
      WordLoad(WO(mk,7), w1[3]);
    } else {
      w1[2]=w1[3]=0;
    }
  } else {
    w1[0]=w1[1]=w1[2]=w1[3]=0;
  }
  w1[0]^=t0; w1[1]^=t1; w1[2]^=t2; w1[3]^=t3;
  t0=w1[0];  t1=w1[1];  t2=w1[2];  t3=w1[3];
  
  q = (q==2)? 0 : (q+1);
  t0^=KRK[q][0]; t1^=KRK[q][1]; t2^=KRK[q][2]; t3^=KRK[q][3];
  FE;
  
  t0^=w0[0]; t1^=w0[1]; t2^=w0[2]; t3^=w0[3];
  w2[0]=t0; w2[1]=t1; w2[2]=t2; w2[3]=t3;
    
  q = (q==2)? 0 : (q+1);
  t0^=KRK[q][0]; t1^=KRK[q][1]; t2^=KRK[q][2]; t3^=KRK[q][3];
  FO;
  w3[0]=t0^w1[0]; w3[1]=t1^w1[1]; w3[2]=t2^w1[2]; w3[3]=t3^w1[3];

  GSRK(w0, w1, 19);  
  GSRK(w1, w2, 19);
  GSRK(w2, w3, 19);
  GSRK(w3, w0, 19);
  GSRK(w0, w1, 31);
  GSRK(w1, w2, 31);
  GSRK(w2, w3, 31);
  GSRK(w3, w0, 31);
  GSRK(w0, w1, 67);  
  GSRK(w1, w2, 67);
  GSRK(w2, w3, 67);
  GSRK(w3, w0, 67);
  GSRK(w0, w1, 97);
  if (keyBits > 128) {  
    GSRK(w1, w2, 97);
    GSRK(w2, w3, 97);
  }
  if (keyBits > 192) {
    GSRK(w3, w0,  97);
    GSRK(w0, w1, 109);
  }
  return (keyBits+256)/32; 
}

/* 복호화 라운드 키 생성. const Byte *mk: 마스터 키, Byte *rk: 라운드 키, int keyBits: 마스터 키의 길이 */
int DecKeySetup(const Byte *mk, Byte *rk, int keyBits) {
  Word *a, *z;
  int rValue;
  register Byte sum;
  register Word t0, t1, t2, t3;
  Word s0, s1, s2, s3;
  
  rValue=EncKeySetup(mk, rk, keyBits);
  a=(Word *)(rk);  z=a+rValue*4;
  t0=a[0]; t1=a[1]; t2=a[2]; t3=a[3];
  a[0]=z[0]; a[1]=z[1]; a[2]=z[2]; a[3]=z[3];
  z[0]=t0; z[1]=t1; z[2]=t2; z[3]=t3;
  a+=4; z-=4;
  
  for (; a<z; a+=4, z-=4) {
    WordM1(a[0],t0); WordM1(a[1],t1); WordM1(a[2],t2); WordM1(a[3],t3);
  
    MM(t0,t1,t2,t3) P(t0,t1,t2,t3) MM(t0,t1,t2,t3)
      s0=t0; s1=t1; s2=t2; s3=t3;
    WordM1(z[0],t0); WordM1(z[1],t1); WordM1(z[2],t2); WordM1(z[3],t3);
  
    MM(t0,t1,t2,t3) P(t0,t1,t2,t3) MM(t0,t1,t2,t3)
      a[0]=t0; a[1]=t1; a[2]=t2; a[3]=t3;
    z[0]=s0; z[1]=s1; z[2]=s2; z[3]=s3;
  }
  WordM1(a[0],t0); WordM1(a[1],t1); WordM1(a[2],t2); WordM1(a[3],t3);
  MM(t0,t1,t2,t3) P(t0,t1,t2,t3) MM(t0,t1,t2,t3)
    z[0]=t0; z[1]=t1; z[2]=t2; z[3]=t3;
    
  return rValue;
}


/*************** Arduino Setting *************************************************/
int   loop_num = 0;
long  total_enc_setup=0, total_enc_time=0, total_dec_setup=0, total_dec_time=0;    

u8    t_cipher[16] = // Test Vector Cipher Text
{ 0xc6, 0xec, 0xd0, 0x8e, 0x22, 0xc3, 0x0a, 0xb0, 0xb2, 0x15, 0xcf, 0x74, 0xe2, 0x07, 0x5e, 0x6e };  
     
void  setup() {
  Serial.begin(9600);    // for Debug
  Serial.println(" ");  
  Serial.println("20181013 YouKyoung KISA ARIA-32bit Code Start..");  Serial.println(" ");
}
 
void  loop() {  
  ARIA_test();   
}

void ARIA_test()
{
    Byte rk[16*17], c[16];
    Byte mk[32]={0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    Byte p[16]={0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa, 0x11, 0x11, 0x11, 0x11, 0xbb, 0xbb, 0xbb, 0xbb};									

    if( loop_num == 0 )    // Display one time 
    {   
        Serial.print("Secret Key:          ");
        for(int i=0; i < Key_Size/8; i++) { Serial.print(mk[i], HEX);  Serial.print(", "); }  Serial.println(" ");
        
        Serial.print("Input Plain Text:    ");
        for(int i=0; i <16; i++) { Serial.print(p[i], HEX);  Serial.print(", "); }  Serial.println(" ");

        EncKeySetup(mk, rk, Key_Size);
        Crypt(p, Key_Size/32 + 8, rk, c);

        Serial.print("Calculated Cipher:   ");
        for (int i=0;i<16;i++)    
        {   Serial.print(c[i], HEX);  Serial.print(", ");  }   Serial.println(" ");
        
        Serial.print("TestVector Cipher:   ");
        for (int i=0;i<16;i++) 
        { Serial.print(t_cipher[i], HEX);  Serial.print(", "); } Serial.println(" ");
        
        DecKeySetup(mk, rk, Key_Size);
        Crypt(c, Key_Size/32 + 8, rk, p);
  
        Serial.print("Decrypted Text :     ");
        for (int i=0;i<16;i++)    
        {   Serial.print(p[i], HEX);  Serial.print(", ");  }   Serial.println(" ");

        loop_num++;
    }
    
    delay(1000);    // 1 second
}
