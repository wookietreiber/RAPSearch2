#include "HashSearch.h"
#include <iostream>
#include "paras.h"
using namespace std;


#define OPTION_QUERY "q"
#define OPTION_SUBJECT "d"
#define OPTION_OUTPUT "o"
#define OPTION_EVAL "e"
#define OPTION_THREADNUM	"z"
#define OPTION_MAXHIT "v"
#define OPTION_MAXALN "b"
#define OPTION_QUERYTYPE "t"
#define OPTION_PRINTEMPTY "p"
#define OPTION_GAPEXT "g"
#define OPTION_ACCELERATE "a"
#define OPTION_HSSP "w"
#define OPTION_MINLEN "l"
#define OPTION_XML "x"
#define	Program	"rapsearch"
#define	Version "2.16"

void printUsage(char *error);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printUsage("");
        return 1;
    }

    char* szDbHash = NULL;
    char* szQrFile = NULL;
    char* szOutFile = NULL;
	int nThreadNum = 0;
	double dLogEvalueCut = DEFAULT_LOGEVALTHRESH;
	int	nMaxHitPer = 500;
	int	nMaxAlnPer = 100;
	int nQueryType = 0;	//0:unknown, 1:nt, 2:aa
	bool bPrintEmpty = false;
	bool bGapExt = true;
	bool bAcc = false;
	bool bHssp = false;
	int nMinLen = 0;
	int bXml = false;

    int	i, k;
    for (i = 0; i < argc; i ++)
    {
        if(argv[i][0] != '-') continue;
        if(argv[i][1] == OPTION_QUERY[0] && argc > i + 1)  // string : query file (FASTA format)
        {
            szQrFile = argv[i+1];
        }
        else if(argv[i][1] == OPTION_SUBJECT[0] && argc > i + 1) // string : subject file (base name only)
        {
            szDbHash = argv[i+1];
        }
        else if(argv[i][1] == OPTION_OUTPUT[0] && argc > i + 1) // string : output file name
        {
            szOutFile = argv[i+1];
        }
		else if(argv[i][1] == OPTION_THREADNUM[0] && argc > i + 1) // string : suffix array file (binary file)
		{
			sscanf(argv[i+1], "%d", &nThreadNum);
		}
		else if(argv[i][1] == OPTION_EVAL[0] && argc > i + 1) // evalue 
		{
			sscanf(argv[i+1], "%lf", &dLogEvalueCut);
		}
		else if(argv[i][1] == OPTION_MAXHIT[0] && argc > i + 1) // maxhit 
		{
			sscanf(argv[i+1], "%d", &nMaxHitPer);
		}
		else if(argv[i][1] == OPTION_MAXALN[0] && argc > i + 1) // maxaln 
		{
			sscanf(argv[i+1], "%d", &nMaxAlnPer);
		}
		else if(argv[i][1] == OPTION_QUERYTYPE[0] && argc > i + 1) // query type 
		{
			char cType = 'n';
			sscanf(argv[i+1], "%c", &cType);
			if ('n' == cType || 'N' == cType)
			{
				nQueryType = 1;
			}
			else if ('a' == cType || 'A' == cType)
			{
				nQueryType = 2;
			}
			else if ('q' == cType || 'Q' == cType)
			{
				nQueryType = 3;
			}
			else
			{
				nQueryType = 0;
			}
		}
		else if(argv[i][1] == OPTION_PRINTEMPTY[0]) // print query without hits 
		{
			char cPrint = 'f';
			sscanf(argv[i+1], "%c", &cPrint);
			if ('t' == cPrint || 'T' == cPrint)
			{
				bPrintEmpty = true;
			}
			else
			{
				bPrintEmpty = false;
			}
		}
		else if(argv[i][1] == OPTION_GAPEXT[0]) // gap extension 
		{
			char cGapExt = 't';
			sscanf(argv[i+1], "%c", &cGapExt);
			if ('f' == cGapExt || 'F' == cGapExt)
			{
				bGapExt = false;
			}
			else
			{
				bGapExt = true;
			}
		}
		else if(argv[i][1] == OPTION_ACCELERATE[0]) // accelerate mode 
		{
			char cAcc = 'f';
			sscanf(argv[i+1], "%c", &cAcc);
			if ('f' == cAcc || 'F' == cAcc)
			{
				bAcc = false;
			}
			else
			{
				bAcc = true;
			}
		}
		else if(argv[i][1] == OPTION_HSSP[0]) // HSSP criteria 
		{
			char cHssp = 'f';
			sscanf(argv[i+1], "%c", &cHssp);
			if ('f' == cHssp || 'F' == cHssp)
			{
				bHssp = false;
			}
			else
			{
				bHssp = true;
			}
		}
		else if(argv[i][1] == OPTION_MINLEN[0] && argc > i + 1) // min alignment length 
		{
			sscanf(argv[i+1], "%d", &nMinLen);
		}
		else if(argv[i][1] == OPTION_XML[0]) // HSSP criteria 
		{
			char cXml = 'f';
			sscanf(argv[i+1], "%c", &cXml);
			if ('f' == cXml || 'F' == cXml)
			{
				bXml = false;
			}
			else
			{
				bXml = true;
			}
		}
    }

    if (!szQrFile)	printUsage("Error: No query");
    if (!szDbHash)	printUsage("Error: No database");
    if (!szOutFile)	printUsage("Error: No output");

    time_t jobstart = time(NULL);

    printf("QueryFileName %s\n", szQrFile);

	CHashSearch hs(nThreadNum);
	hs.Process(szDbHash, szQrFile, szOutFile, dLogEvalueCut, nMaxAlnPer, nMaxHitPer, nQueryType, bPrintEmpty, bGapExt, bAcc, bHssp, nMinLen, bXml);

    printf(">>>Main END\n");
    time_t jobfinished = time(NULL);
    double	timeused = difftime(jobfinished, jobstart);
    printf("Time used: %d min (%.1f hours)\n", int(timeused / 60), timeused / 3600.0);

    return 0;
}


void printUsage(char *error)
{
    fprintf(stderr, "%s\n", error);
    fprintf(stderr,
            "%s v%s: Fast protein similarity search tool for short reads\n"
            "-------------------------------------------------------------------------\n"
            " Options: \n"
            "\t-" OPTION_QUERY        " string : query file (FASTA format)\n"
            "\t-" OPTION_SUBJECT      " string : database file (**base name only**, with full path)\n"
            "\t-" OPTION_OUTPUT       " string : output file name [stdout]\n"
            "\t-" OPTION_THREADNUM         " int  : number of threads [default: %d]\n"
            "\t-" OPTION_EVAL         " double  : threashold of log_10(E-value) [default: %.1f]\n"
            "\t-" OPTION_MINLEN         " int  : threashold of minimal alignment length [default: %d]\n"
			"\t-" OPTION_MAXHIT       " int    : number of database sequences to show one-line descriptions [default: %d]. If it's -1, all results will be shown.\n"
			"\t-" OPTION_MAXALN       " int    : number of database sequence to show alignments [default: %d]. If it's -1, all results will be shown.\n"
			"\t-" OPTION_QUERYTYPE       " char    : type of query sequences [u/U:unknown, n/N:nucleotide, a/A:amino acid, q/Q:fastq, default: %s]\n"
			"\t-" OPTION_PRINTEMPTY       " char    : output ALL/MATCHED query reads into the alignment file [t/T: output all query reads, f/F: output matched reads, default: %s]\n"
			"\t-" OPTION_GAPEXT       " char    : perform gap extension to speed up [t/T: perform gap extension, f/F: do not perform gap extension, default: %s]\n"
			"\t-" OPTION_ACCELERATE       " char    : fast mode (10~30 fold) [t/T: perform fast search, f/F: perform normal search, default: %s]\n"
			"\t-" OPTION_HSSP       " char    : perform HSSP criteria instead of evalue criteria [t/T: perform HSSP criteria, f/F: perform evalue criteria, default: %s]\n"
			"\t-" OPTION_XML       " char    : print hits in xml format [t/T: print hits in xml format, f/F: not print hits in xml format, default: %s]\n"
            "-------------------------------------------------------------------------\n"
            "example> %s -" OPTION_QUERY " query.fa -" OPTION_SUBJECT " nr -" OPTION_OUTPUT " output_file\n\n"
            ,
            Program, Version, 1, 1.0, 0, 500, 100, "u", "f", "t", "f", "f", "f", Program
           );
    exit(-1);
}