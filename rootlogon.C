{
    // User Defining Area //
    TString libFilename = "libqupid";
  
    std::cout << "===================-=====================" <<std::endl<<std::endl;
    std::cout << "Quasi-Signal Generation Model for Pixelized Detector" <<std::endl;
  
    TString DirName = TString(gSystem->GetWorkingDirectory());
    if( !(DirName.EndsWith("/build") || DirName.EndsWith("/build/")) ){
        DirName = DirName + TString("/build/");
    }

    if( !(DirName.EndsWith("/")) ){
        DirName = DirName + TString("/");
    }
  
    TString libIncludePath = TString::Format("%sinclude", DirName.Data());
    TString libLoadPath = TString::Format("%s%s", DirName.Data(), libFilename.Data());
    TString libVisLoadPath = TString::Format("%slibqupidVis", DirName.Data());

    gInterpreter->AddIncludePath(libIncludePath);
    gInterpreter->Load(libLoadPath, true);
    // gInterpreter->Load(libLoadPath, false);
    gSystem->Load(libLoadPath);

    // optional visualization module (present only for ROOT-enabled builds)
    gInterpreter->Load(libVisLoadPath, true);
    gSystem->Load(libVisLoadPath);

    std::cout<<"Include Path: " <<libIncludePath<<std::endl;
    std::cout<<"Library Path: " <<libLoadPath<<std::endl;
    std::cout<<"Vis Library Path: " <<libVisLoadPath<<std::endl;

    std::cout<<std::endl;

    std::cout<<"qupid is loaded for TInterpreter."<<std::endl;
    std::cout<<"Please consider whether you are in same folder with libqupid file."<<std::endl;

    gInterpreter->LoadMacro("./QPStyle.cpp");
    SetStyle();

    std::cout <<std::endl<< "===================-=====================" <<std::endl;
}
