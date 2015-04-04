#include <ysclass.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp=fopen("datafile.txt","r");
	if(NULL!=fp)
	{
		YsString str;
		while(NULL!=str.Fgets(fp))
		{
			YsArray <YsString> args;
			str.Arguments(args);
			if(0<args.GetN())
			{
				if(0==strcmp("mass",args[0]) && 2<=args.GetN())
				{
					const double m=atof(args[1]);
					printf("m %lf\n",m);
				}
				else if(0==strcmp("sigma",args[0]) && 2<=args.GetN())
				{
					const double sigma=atof(args[1]);
					printf("sig %lf\n",sigma);
				}
				else if(0==strcmp("particle",args[0]) && 4<=args.GetN())
				{
					const double x=atof(args[1]);
					const double y=atof(args[1]);
					const double z=atof(args[1]);
					printf("P %lf %lf %lf\n",x,y,z);
				}
			}
		}
		fclose(fp);
	}
	return 0;
}

