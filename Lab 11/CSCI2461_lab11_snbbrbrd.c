#include<stdio.h>

	int main()
	{
		    FILE *rd_file1;
		    FILE *rd_file2;
		    FILE *wrt_file;
		    
    		char buf[1000];

    		rd_file1 =fopen("input1.txt", "r");
    		rd_file2 =fopen("input2.txt", "r");
    		wrt_file =fopen("output.txt", "w");
    		
    		
    		if (!rd_file1 || !rd_file2 || !wrt_file)
        		return 1;

    		while (fgets(buf,1000, rd_file1)!=NULL)
        		fprintf(wrt_file, "%s",buf);
        		
        	while (fgets(buf,1000, rd_file2)!=NULL)
        		fprintf(wrt_file, "%s",buf);

			fclose(rd_file1);
			fclose(rd_file2);
			fclose(wrt_file);
			
    		return 0;
	}