// Dump the test6 Monte Carlo tree to a tab-separated file for regression
// comparison: columns amp, x, y, npix with full double precision.
// Usage: root -b -q -l macro/dump_test6.C
//        (reads ./test6_output.root, writes ./test6_dump.tsv)

void dump_test6(const char* infile = "test6_output.root",
                const char* outfile = "test6_dump.tsv"){
    TFile f(infile);
    if(f.IsZombie()){
        fprintf(stderr, "dump_test6: cannot open %s\n", infile);
        gSystem->Exit(1);
    }
    TTree* tree = (TTree*) f.Get("tree");
    if(!tree){
        fprintf(stderr, "dump_test6: no tree in %s\n", infile);
        gSystem->Exit(1);
    }

    struct { double x, y; } pos;
    double amp;
    int npix;
    tree->SetBranchAddress("amp",  &amp);
    tree->SetBranchAddress("pos",  &pos);
    tree->SetBranchAddress("npix", &npix);

    FILE* out = fopen(outfile, "w");
    if(!out){
        fprintf(stderr, "dump_test6: cannot write %s\n", outfile);
        gSystem->Exit(1);
    }
    const Long64_t n = tree->GetEntries();
    for(Long64_t i=0; i<n; i++){
        tree->GetEntry(i);
        fprintf(out, "%.17g\t%.17g\t%.17g\t%d\n", amp, pos.x, pos.y, npix);
    }
    fclose(out);
    printf("dump_test6: %lld entries -> %s\n", n, outfile);
}
