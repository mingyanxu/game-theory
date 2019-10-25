#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <time.h>
#include <stdlib.h>
#include<iostream>
using namespace std;

void main()
{
	int i, j, k, round, randomx, hit, m, n, N;
	int loner, cooperator, defector;
	int nc, nd, nl;
	int i01, i02, i11, i12, j01, j02, j11, j12;
	int flag;     //默认Loner标志
	float w1, w2, r, K, c;
	float payoff;
	float avpc, avpd;
	float initC, initD;
	float ac[10000];
	float ad[10000];
	float al[10000];
	float pc[10000];
	float pd[10000];
	float avc, avd, avl, apc, apd;
	float standard;

	FILE *fp, *fp2;
	struct person {
		int state;
		float payoffoftheround;
		float totalpayoff;
		float avpayoff;
		float prox;
		float proy;
		float proz;
		float standard;
		int engage;
	}mem[100][100];

	//初始化
	//w1=0.01;//对收益差的敏感度
	//w2=1;   //随机选择的概率
	r = 3.0;    //收益率
	K = 1.0;
	w1 = 0.01;
	w2 = 1;
	c = 1;//投资

	initC = 1.0 / 3;
	initD = 1.0 / 3;
	//for (K = 0.1; K <= 2.0; K = K+0.2111)
	//{
	//for (w1 = 0.01; w1 <= 100;w1=w1*1.26638)
	//for (r = 2.0; r <= 5.0; r = r + 0.1)
	//{
		/*
		printf("输入初始时合作者的百分比:");
		scanf("%f", &initC);
		printf("输入初始时背叛者的百分比:");
		scanf("%f", &initD);

		if (initC + initD>1) { printf("Enter Error!!"); _getch(); exit(0); }
		*/

		srand((unsigned)time(NULL));  //随机数初始化
		if ((fp = fopen("detail.txt", "a")) == NULL)
		{
			printf("ERROR#1:cannot open the file!\n");
			_getch();
			exit(0);

		}
		else
			printf("File detail.txt is created!\n");

		if ((fp2 = fopen("payoff.txt", "a")) == NULL)
		{
			printf("ERROR#1:cannot open the file!\n");
			_getch();
			exit(0);

		}
		


		for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++)
		{
			mem[i][j].engage = 0;
			mem[i][j].standard = 1.0;
			mem[i][j].prox = initC;
			mem[i][j].proy = initD;
			mem[i][j].proz = 1 - mem[i][j].prox - mem[i][j].proy;
			mem[i][j].avpayoff = 0;
			mem[i][j].payoffoftheround = 0;
			mem[i][j].totalpayoff = 0;
		}//混合策略初始化

		//round start
		for (round = 0; round < 60000; round++)
		{
			if (round % 1000 == 0)
				printf("round:%d\n", round);
			//获得策略
			for (i = 0; i < 100; i++)
			for (j = 0; j < 100; j++)
			{
				if (rand() % 10000 < w2)
					mem[i][j].state = rand() % 3;
				else
				{
					randomx = rand() % 1000;
					if (randomx < mem[i][j].prox * 1000) mem[i][j].state = 1;
					else if (randomx < (mem[i][j].prox + mem[i][j].proy) * 1000) mem[i][j].state = 2;
					else mem[i][j].state = 0;
				}
			}


			//计算个体的收益以及混合策略更新
			avpc = avpd = 0;
			for (hit = 0; hit < 10000; hit++)
			{
				m = (int)(100.0*rand() / (RAND_MAX + 1.0));//生成随机数0-99
				n = (int)(100.0*rand() / (RAND_MAX + 1.0));
				i = m;
				j = n;

				i01 = i - 1; if (i01<0) i01 += 100;

				j01 = j - 1; if (j01<0) j01 += 100;

				i11 = i + 1; if (i11>99) i11 -= 100;

				j11 = j + 1; if (j11>99) j11 -= 100;



				nc = nd = nl = 0;

				switch (mem[i][j01].state)   //上近邻
				{
				case 0:
					nl++;
					break;
				case 1:
					nc++;
					break;
				case 2:
					nd++;
					break;
				default:
					break;
				}

				switch (mem[i01][j].state)   //左近邻
				{
				case 0:
					nl++;
					break;
				case 1:
					nc++;
					break;
				case 2:
					nd++;
					break;
				default:
					break;
				}

				switch (mem[i][j11].state)   //下近邻
				{
				case 0:
					nl++;
					break;
				case 1:
					nc++;
					break;
				case 2:
					nd++;
					break;
				default:
					break;
				}

				switch (mem[i11][j].state)   //右近邻
				{
				case 0:
					nl++;
					break;
				case 1:
					nc++;
					break;
				case 2:
					nd++;
					break;
				default:
					break;
				}


				if (nl + nc + nd != 4) printf("ERROR#2:邻居公式有错误\n");


				//计算收益并对混合策略进行更新
				payoff = 0; flag = 0;
				//standard = 0;

				if (nl == 4 || mem[i][j].state == 0)     //loner强制转化或各自中心本来就是loner
				{
					mem[i][j].state = 0;
					payoff += K;
					//mem[i][j].payoffoftheround = K;
					mem[i][j].totalpayoff += K;
					//mem[i][j].avpayoff = mem[i][j].totalpayoff / (round+1);
					

				}
				else  //不发生loner的强制转化
				{
					mem[i][j].engage++;
					if (mem[i][j].state == 1)    //如果中心格子为合作者
					{

						payoff += (float)r*c*(1 + nc) / (1 + nc + nd) - c;
						//mem[i][j].payoffoftheround = (float)r*c*(1 + nc) / (1 + nc + nd) - c;
						//mem[i][j].totalpayoff += (float)r*c*(1 + nc) / (1 + nc + nd) - c;
						//mem[i][j].avpayoff = (float)mem[i][j].totalpayoff/(round+1);
						
						//if (mem[i][j].avpayoff > 1)
							//standard = mem[i][j].avpayoff;错误，standard更新应在一轮结束后
						//else
							//standard = 1;
						//cout << standard << endl;

						//*******判断是否自动转变为Loner********
						//如果不自动转化为Loner进入下面的计算
						avpc += payoff;
						if (payoff >mem[i][j].standard)         //如果合作者收益高于Loner 混合策略的演化
						{
							if (mem[i][j].proz <= w1*(payoff - mem[i][j].standard))
							{
								mem[i][j].proz = 0;
								mem[i][j].prox = 1 - mem[i][j].proy;
							}
							else
							{
								mem[i][j].proz -= w1*(payoff - mem[i][j].standard);
								mem[i][j].prox = 1 - mem[i][j].proy - mem[i][j].proz;
							}
						}
						else if (payoff < mem[i][j].standard)
						{
							if (mem[i][j].prox <= w1*(mem[i][j].standard - payoff))
							{
								mem[i][j].prox = 0;
								mem[i][j].proz = 1 - mem[i][j].proy;
							}
							else
							{
								mem[i][j].proz += w1*(mem[i][j].standard - payoff);
								mem[i][j].prox = 1 - mem[i][j].proy - mem[i][j].proz;
							}
						}
					}



					else
						//如果中心格子为背叛者
					{
						payoff += (float)r*c*nc / (1 + nc + nd);
						//mem[i][j].payoffoftheround = (float)r*c*(1 + nc) / (1 + nc + nd) ;
						//mem[i][j].totalpayoff += (float)r*c*(1 + nc) / (1 + nc + nd) ;
						//mem[i][j].avpayoff = (float)mem[i][j].totalpayoff / (round+1);
						//if (mem[i][j].avpayoff > 1)
							//standard = mem[i][j].avpayoff;
					//	else
					//		standard = K;

						//*******判断是否自动转变为Loner********
						avpd += payoff;
						if (payoff > mem[i][j].standard)         //如果背叛者收益高于Loner
						{
							if (mem[i][j].proz <= w1*(payoff - mem[i][j].standard))
							{
								mem[i][j].proz = 0;
								//mem[i][j].proy=1-mem[i][j].proz;错误
								mem[i][j].proy = 1 - mem[i][j].prox;
							}
							else
							{
								mem[i][j].proz -= w1*(payoff - mem[i][j].standard);
								mem[i][j].proy = 1 - mem[i][j].prox - mem[i][j].proz;
							}
						}
						else if (payoff < mem[i][j].standard)
						{
							if (mem[i][j].proy <= w1*(mem[i][j].standard - payoff))
							{
								mem[i][j].proy = 0;
								mem[i][j].proz = 1 - mem[i][j].prox;
							}
							else
							{
								mem[i][j].proz += w1*(mem[i][j].standard - payoff);
								mem[i][j].proy = 1 - mem[i][j].prox - mem[i][j].proz;
							}
						}

					}//混合策略更新完毕
					
				}
				mem[i][j].totalpayoff += payoff;
				mem[i][j].avpayoff = mem[i][j].totalpayoff / (round + 1);

				if (mem[i][j].avpayoff > K)

					mem[i][j].standard = mem[i][j].avpayoff;
						
				else
							mem[i][j].standard = K;

					//cout << mem[i][j].standard<<endl;
				//standard更新完毕

			}//hit循环

			//****************************		
			//计算各策略的人数
			loner = cooperator = defector = 0;
			for (i = 0; i < 100; i++)
			for (j = 0; j < 100; j++)
			{
				switch (mem[i][j].state)
				{
				case 0:
					loner++;
					break;
				case 1:
					cooperator++;
					break;
				case 2:
					defector++;
					break;
				default:
					printf("ERROR#?\n");
					_getch();
					break;
				}
			}

			//if ((round + 1) % 60000 == 0)
			//{
			//	for (i = 0; i<100; i++)
			//	for (j = 0; j<100; j++)
			//
			//		fprintf(fp, "%d\t%d\t%d\n", i + 1, j + 1, mem[i][j].state);
			//}


			//fprintf(fp, "%d\t%f\t%f\t%f\n", round, (float)loner / 10000, (float)cooperator / 10000, (float)defector / 10000);
			//******************************
			if (round >= 50000)
			{
				ac[round -50000] = (float)cooperator / 10000;
				ad[round - 50000] = (float)defector / 10000;
				al[round - 50000] = (float)loner / 10000;
				if (cooperator == 0)
					pc[round - 50000] = 0;
				else
					pc[round - 50000] = (float)avpc / cooperator;
				if (defector == 0)
					pd[round - 50000] = 0;
				else
					pd[round - 50000] = (float)avpd / defector;

			}if (round % 500 == 0)
			{
				ac[round / 500] = (float)cooperator / 10000;
				ad[round / 500] = (float)defector / 10000;
				al[round / 500] = (float)loner / 10000;
				if (cooperator == 0)
				{
					pc[round / 500] = 0;
				}
				else
					pc[round / 500] = (float)avpc / cooperator;
				if (defector == 0)
					pd[round / 500] = 0;
				else
					pd[round / 500] = (float)avpd / defector;

				//fprintf(fp3, "%d\t%f\t%f\t%f\t%f\t%t%f\n", round, ac[round / 500], ad[round / 500], al[round / 500]);
			}


		}//ROUND FINISH

		avc = 0;
		apc = 0;
		for (i = 0; i < 10000; i++)
		{
			avc += ac[i];
			apc += pc[i];
		}
		avc = avc / 10000;
		apc = apc / 10000;

		avd = 0;
		apd = 0;
		for (i = 0; i < 10000; i++)
		{
			avd += ad[i];
			apd += pd[i];
		}
		avd = avd / 10000;
		apd = apd / 10000;

		avl = 0;
		for (i = 0; i < 10000; i++)
		{
			avl += al[i];
		}
		avl = avl / 10000;
		fprintf(fp, "%f\t%f\t%f\n", avc, avd, avl);
		/*
		if (cooperator == 0) apc = 0;
		else              apc = (float)apc / cooperator;
		if (defector == 0)   avpd = 0;
		else              apd = (float)apd / defector;
		*/
		fprintf(fp2, "%f\t%f\n", apc, apd);

		fclose(fp);
		fclose(fp2);
	}

	