
#ifndef YOURACTIONINITIALIZATION_HH
#define YOURACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include <string>

class YourActionInitialization : public G4VUserActionInitialization {

public:

    YourActionInitialization(std::string ofilename);
    ~YourActionInitialization() override;

    void Build() const override;

    void BuildForMaster() const override;

  private:
    std::string _ofilename;
};

#endif
