/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically. //算数右移
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~x&~y)&~(x&y); //数电的东西 这算7个操作
  // return 2;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  int x = 0x01;
  x = x<<31;
  return x;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  int i = x+1; //0xffffff+1是0x00，！0=1 最终的结果是0
  x = x+i; //即便是补码，二补码相加还是进行二进制加法
  //利用最大值的特性，正好通过这样的操作可以把它归结到0
  x=~x;
  //下边的这两行是防止0xfffffffff对结果造成干扰
  i = !i;
  x=x+i;
  return !x;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int part1 = 0xAA;
  int part2 = part1<<8;
  int part3 = part1+part2;
  int part4 = part3 + (part3<<16); //要注意加法运算的优先级是高于移位运算符的
  //不断组合得到大的掩码

  return !((x&part4)^part4);


  //不要像下边这样分开进行，要充分利用上一步的结果
  // int result1 = part1&x;
  // int result2 = part2&x;
  // int result3 = part3&x;
  // int result4 = part4&x;

  // int result = 0xAA&x;
  // result = result+result^0xAA;
  // x = x>>8;
  // result = result+(0xAA&x)^0xAA;

  // x = x>>8;
  // result = 0xAA&x;
  // result = result+result^0xAA;

  // x = x>>8;
  // result = 0xAA&x;
  // result = result+result^0xAA;
  // return !result;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x)+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  //构造lower和upper双重验证，只有二者所验证的区间进过处理都为0才认为处于该范围内
  //补码加减法就是正常的二进制数加减，然后截断到int的取值范围内
  int upper = ~((1<<31)|(0x39));
  int lower = (~0x30)+1;

  int upperplusx = upper + x;
  int lowerplusx = lower + x;
  return (!(upperplusx>>31))&(!(lowerplusx>>31)); //int 转bool 只有在int为0时bool为0 int不为0时bool为1

}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int ifx = ((!(!x))<<31)>>31; //!果然是转化为bool的 两个！之后就可以规范一下数
  int first = ifx&y;

  int second = (~ifx)&z;
  return first|second;
  
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int isequal = !(x^y); //返回1 

  //实际上只需要很简单的当符号不同时正数为大，当符号相同时相减判断符号即可。符号相同时相减一定不会溢出。
  int issamesign = !(((x>>31)&1)^((y>>31)&1)); 
  int xminusy = x+((~y)+1);
  int second = (issamesign&((xminusy>>31)&1))|(!issamesign&!((y>>31)&1));//这个就可以实现类似于if的效果

  return isequal|second;


  //下面注释掉的这些失败的代码试图处理负极值的情况，或者说试图处理溢出的情况，这是失败的，因为溢出时符号位是多少是不确定的
  // int ispositivex = !((x>>31)&1);
  // int ispositivey = !((y>>31)&1);

  // int second = !ispositivex&(ispositivey);
  // int third = ((x+((~y)+1))>>31)&0x01; 
  // int isminy = !(y^(1<<31)); //如果y是最小值，ismin是1 否则是0
  // return (third|second|isequal)&(!((!isequal)&isminy)); 

  // int second = ((x+((~y)+1))>>31)&0x01; //加-y这种是不行的，比如说当x为-127，y为128，这样一减的话会溢出
  
  // return (isequal|second)&(!((!isequal)&isminy));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  //利用当逻辑值为1时的数的相反数的符号位不同这一概念
  //但是需要注意，当取负数最小值时，取相反数的符号相同，但是不影响最终结果,因为是取或的
  int negx = (~x)+1;
  return ((((x|negx)>>31)&1)+1)&1;

}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  //要想找到最小能够代表位数的值，对于正数，需要找最高位为1的位数
  //对于负数，需要找最高位为0的位数
  int b16,b8,b4,b2,b1,b0; //判断高n位是否有1值 
  int ispositive = ~(x>>31); //当为正数时，所有位都为1 当为负数时，所有位都为0
  x = (ispositive&x)|((~ispositive)&(~x)); //正数不变，负数取反.这样就把整个问题规约到了一个问题上

  //下面的思路非常巧妙 把问题分解为多个子问题 有点二分的思想
  b16 = (!!(x>>16))<<4; //当高16位存在1时，记b16=16 否则b16 = 0
  x = x>>b16;//当b16确实有值时，右移16位，判断到底是高8位还是低八位 否则不移动，直接判断低16位
  b8 = (!!(x>>8))<<3;// 当高8位存在1时，记b8 = 8，否则b8=0
  x = x>>b8;
  b4 = (!!(x>>4))<<2;
  x = x>>b4;
  b2 = (!!(x>>2))<<1;
  x = x>>b2;
  b1 = !!(x>>1); //这一步是必须的，因为不是所有数在上述步骤中都要右移。当上述所有步都要右移时b1一定为0
  x = x>>b1;
  b0 = x;

  return b0+b1+b2+b4+b8+b16+1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned exp = (uf<<1)>>24;
  unsigned result = uf;
  unsigned sign = (uf>>31)<<31;
  //注意下边if的顺序，顺序不对的话会出现错误
  if(exp==0) //说明exp段为0
  {
    result = result<<1;
    result = result|sign;
  }
  else if (exp==0xff) //当是这个时，数要不是Nan，要不是inf 最后都会返回本身
  {

  }
  else if (exp!=0xfe) //当是这个时，说明是正常的，×2只需要将exp位+1即可
  {
    result = result + (1<<23);
  }
  else if (exp==0xfe)//当exp等于0xfe时,任何数×2都会溢出 
  {
    result = result + (1<<23);
    result = (result>>23)<<23;
  }
  
  return result;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  //把float分解成3部分
  int exp = (uf>>23)&0xff;
  int frac = uf & 0x7fffff;
  int sign = uf&(1<<31);

  int frac1 = frac|0x800000;
   //扩展尾数，在最高位的最高位扩展一位1，
  int biasedExp = exp-127; //加了偏置的指数位

  if (exp==0xff) return 0x80000000; //溢出或者nan时返回固定数
  if (exp==0x00) return 0;//当exp为0时，整个浮点数一定不会超过0.5，返回0

  
 

  if(biasedExp>31) return 0x80000000; 
  //把浮点数转化为整数的过程看成是一个基于frac1的小数点移动的过程，初始时小数点在frac1的最高位的后边
  //然后乘以2^biasedExp就相当于把小数点右移biasedExp 当右移32次时浮点数表示的值是33位的，4字节的int已经不足以表示那个值了，出现了溢出。
  //因此小数点右移31位是临界值
  else if (biasedExp<0) return 0;
  //当biasedexp小于0时，意味着小数点要向左移一位，就是说整数部分一定是0，截断小数部分，就返回0。
  //注意这里与中间值向偶数舍入是无关的，向偶数舍入是指当要把高精度小数表示为一低精度小数以便float能表示时，进行向偶数位转换的操作
  //但是这里是直接转换为int，就直接截断就行。

  if (biasedExp>23) frac1 = frac1<<(biasedExp-23);
  //如果大于23的话，相当于把小数点右移大于23位，转化到int表示后需要把frac1左移biasedExp-23，用0补齐
  else frac1 = frac1>>(23-biasedExp); 
  //如果小于等于23的话，相当于把小数点右移<=23位，转换到int表示后需要把小数点后的小数值截断

  if(sign) return ~frac1+1; //这里还是有点问题 再考虑一下(对于溢出的界定) 或者用vs试一下看看结果
  //如果原始的float是负数的话，就对frac1取负数;但需要注意的是，当biasedExp=31,最高值为1时，取负数不是真实的负数，虽然没有溢出，
  //但是结果偏差很大
  else if (frac1>>31) return 0x80000000;
  //如果原始float是正数，biasedExp=31,经过小数点移动的尾数的最高位就到了31位，也就是说
  //int的最高位成了1。如果是负数，则在if语句中已经有了处理，不会出现溢出；否则如果是正数，再判断出最高位是1
  //那么要想表示这个正数的话需要再多加一位，则表明溢出了
  else return frac1;

  //下边这个失败的尝试试图通过按位赋权累加的方法得到结果再截断，这样是不能不借助float得出结果的。
  // int exp = (uf<<1)>>24;
  // int E;
  
  // if (exp==0xff)  return 0x80000000;
  // else if(exp == 0)
  // {
  //   E = 1 - 127;
  //   return 0;
  // }
  // else
  // {
  //   E = exp - 127;
  //   if (E<-1)
  //     return 0;
  // }
  
  // return 1;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  int maxpow = 0xfe - 127; //exp段能表示的最大指数
  int minpow = 1 - 127; //exp段能表示的最小指数
  if(x>maxpow)
  {
    return 0x7f800000;//返回正无穷
  }
  else if (x<minpow)
  {
    return 0;
  }
  else
  {
    return (x+127)<<23;
  }
  
}
