<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>

<script type="text/x-mathjax-config">
MathJax.Hub.Config({
tex2jax: {inlineMath: [['$','$'], ['\(','\)']]}
});
</script>


# 信息安全技术实验报告 实验二

### 16061334 --- 张子扬 --- 2019.04.04

----

## 一、实验目的

理解、掌握分组密码算法的 4 种工作模式：ECB,CBC,CFB,OFB。

## 二、实验内容

1. 调用 DES 分组密码源程序中的相关函数，设计支持 CBC 模式的一个文件加密器，可以加密任意长度的文件，并密文文件可解密还原回原明文文件。
2. 调用 DES 分组密码源程序中的相关函数，设计支持 CFB 模式的一个文件加密器，可以加密任意长度的文件，并密文文件可解密还原回原明文文件。
 
## 三、分组密码算法工作模式基本原理

### 电子密码本 ECB (**E**lectronic **C**ode**B**ook)
#### 模式介绍：

直接利用分组密码对明文的各个分组进行加密，电子密码本是分组密码的基本工作模式。

其加密和解密描述成以下式子:

$$ C_i = E(K, P_i) $$
$$ P_i = E^{-1}(K, C_i) $$

#### 特点

1. 简单和有效；可并行实现；
2. 密文块损坏仅对应明文块损坏；
3. *相同明文产生相同密文，同样信息多次出现造成泄漏，不能隐藏明文的模式信息；*
4. *对明文的主动攻击是可能的，信息块可重放；*
5. 适合于传输短信息。


### 密码分组链接 CBC (**C**ipher **B**lock**C**haining)
#### 模式介绍：

密码分组链模式就是明文要与前面的密文进行异或运算之后再进行加密，也就是每一个分组的加密都依赖于前面的分组，在处理第一个分组的时候与一个初始向量组进行异或运算，初始向量组不需要保密可以以明文的形式与密文一起传送。

其加密和解密描述成以下式子:

$$ C_i = E(K, C_{i-1} \oplus P_i)$$
$$ P_i = E^{-1}(K, C_i) \oplus C_{i-1} $$

#### 特点：

1. 相同明文产生不同密文，需共同初始向量 IV，能隐藏明文的模式信息；不易对明文主动攻击；
2. *密文块损坏导致**两**明文块损坏*；
3. 安全性好于 ECB；
4. *不能并行实现；*
5. 适合长报文传输，可进行用户鉴别，**是大多系统的标准，如 SSL、IPSec**。

### 输出反馈 OFB (**O**utput **F**eed**B**ack)
#### 模式介绍：

分组密码E把寄存器R的状态内容作为明文，并加密为密文，E输出的密文又返回到寄存器R，作为下一次加密的输入，E输出的密文的最右位作为密钥序列的输出，与明文异或实现序列加密，与明文异或实现序列加密。

其加密和解密描述成以下式子:

$$ C_i = P_i \oplus E(K,O_i) $$
$$ P_i = C_i \oplus E(K,O_i) $$

#### 特点：

1. 隐藏了明文模式；
2. 需要共同的移位寄存器初始值IV；
3. 一个单元损坏只影响对应单元；
4. *对明文的主动攻击是可能的,信息块可被替换、重放*；
5. *安全性较 CFB 差*。
6. 可将分组密码当做流密码。

### 密码反馈 CFB (**C**ipher **F**eed**B**ack)
#### 模式介绍：

密码反馈模式工作原理与输出反馈模式基本相同,不同的是反馈到寄存器R的不是E输出的最右位,而是异或加密后的密文。

其加密和解密描述成以下式子:

$$ C_1 = P_1 \oplus MSB_s[E(K,IV)] $$

$$ P_1 = C_1 \oplus MSB_s[E(K,IV)] $$

#### 特点：

1. 隐藏了明文模式；
2. 需要共同的移位寄存器初始值IV；
3. *一个单元损坏影响**多个**单元*。
4. 可将分组密码当做流密码。


## 四、实验过程

在上次实验（实验一）中，我用 **“电⼦子密码本ECB”** 模式实现了一个文件加密器。本次试验我就在上次的代码上做修改已实现这次两个文件加密器，其实大部分的代码都不需要改动，在这次实验报告中，我只展示改动后的代码。

ECB 模式的加密代码：

``` C++
DES_EncryptBlock(group, subKeys, cstr);
```

ECB 模式的解密代码：

``` C++
DES_DecryptBlock(group, subKeys, dstr);
```

- ### CBC 模式文件加密器:
  1. CBC 模式比 EBC 模式多需要一个对块取异或的函数，所以边写异或函数：
   ```C++
   void XOR(ElemType in[8], ElemType overwrite[8]) {
      for(int i=0; i < 8; i++) {
         overwrite[i] = (in[i]^overwrite[i]);
      }
   }
   ```
  2. 随后按照 CBC 模式的加解密公式编写加解密语句：
  3. 加密代码：
   ```C++
   // 在变量声明时对 cstr 赋初始值作为流程中的 IV
   XOR(cstr, group);
   DES_EncryptBlock(group, subKeys, cstr);
   ```
  4. 解密代码：
   ```C++
   // 在变量声明时对 oldgroup 赋初始值作为流程中的 IV
   DES_DecryptBlock(group, subKeys, dstr);
   XOR(oldgroup, dstr);
   for(int i=0; i < 8; i++) {
      oldgroup[i] = group[i];
   }
   ```
  5. 编译程序. 输出结果打印在终端.
  6. 完整代码在附件中, 效果图在 `五、实验结果`
   
- ### CFB 模式文件加密器:
  1. 在编写 CFB 模式的加密器时，为了使函数的逻辑更加清晰，重新写了一下 XOR 函数：
   ```C++
   void XOR(ElemType in1[8], ElemType in2[8], ElemType out[8]) {
      for(int i=0; i < 8; i++) {
         out[i] = (in1[i]^in2[i]);
      }
   }
   ```
  2. 编写循环左移的函数 `LeftN()`, 但是因为不要求移位，这个函数不对数据作处理， 留空。 
  3. 随后按照 CFB 模式的加解密公式编写加解密语句：
  4. 加密代码：
   ```C++
   // 在变量声明时对 cstr 赋初始值作为流程中的 IV
   LeftN(cstr);
   DES_EncryptBlock(cstr, subKeys, zstr);
   XOR(group, zstr, cstr);
   ```
  5. 解密代码：
   ```C++
   // 在变量声明时对 oldgroup 赋初始值作为流程中的 IV
   LeftN(oldgroup); 
   DES_EncryptBlock(oldgroup, subKeys, zstr); 
   XOR(group, zstr, dstr); 
   for(int i=0; i < 8; i++) {
      oldgroup[i] = group[i];
   }
   ```
  6. 可以看到，在 CFB 模式下工作，只需要 `DES_EncryptBlock()` 加密函数即可，加解密都由这个函数来完成。
  7. 编译程序. 输出结果打印在终端.
  8.  完整代码在附件中, 效果图在 `五、实验结果`


## 五、实验结果

- ### 文件加密器加解密效果:
  - 代码文件 `main-cbc.cpp`、`main-cfb.cpp`
  - CFB 加解密图片:
  - ![加解密图片](cfb.png)
  - 图片文件对比:
  - ![图片文件对比](pic.png)
  - CBC 加解密音频:
  - ![加解密音频](cbc.png)
  - 音频文件对比:
  - ![音频文件对比](music.png)
