#include <stdio.h>
#include <stdlib.h>

#define		FOR_LOOP_TEMPLATE(x)	for(int x=1; x<=9; ++x)
#define		FILTER_PASS(x)	if(x==1 || x==6 || x==9) continue;

double func(double i, double j, double k, double l, double m, double n)
{
	return (1/(i*6)+(j/(k*9)+(l/(m*n))));
}

bool isAllDiff(int i, int j, int k, int l, int m, int n)
{
	return (
		(i!=j)&& (i!=k)&& (i!=l)&& (i!=m)&& (i!=n)
		&& (j!=k)&& (j!=l)&& (j!=m)&& (j!=n)
		&& (k!=l)&& (k!=m)&& (k!=n)
		&& (l!=m)&& (l!=n)
		&& (m!=n)
		);
}

int main(int argc, char* argv[])
{
	FOR_LOOP_TEMPLATE(i) {
		FILTER_PASS(i)

		FOR_LOOP_TEMPLATE(j) {
			FILTER_PASS(j)

			FOR_LOOP_TEMPLATE(k) {
				FILTER_PASS(k)

				FOR_LOOP_TEMPLATE(l) {
					FILTER_PASS(l)

					FOR_LOOP_TEMPLATE(m) {
						FILTER_PASS(m)

						FOR_LOOP_TEMPLATE(n) {
							FILTER_PASS(n)

							double result = func(i, j, k, l, m, n);
							
							if( result==1 && isAllDiff(i, j, k, l, m, n) ) {
									printf("1/(%d)x6 + (%d)/(%d)x9 + (%d)/(%d)x(%d) = 1 = %d\n",
										i,j,k,l,m,n, result);
							}
									
						}
					}
				}
			}
		}
	}

	system("pause");

	return 0;
}