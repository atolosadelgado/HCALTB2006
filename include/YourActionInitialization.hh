
#ifndef YOURACTIONINITIALIZATION_HH
#define YOURACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include <string>
class YourInputArgs;
class G4MaterialScanner;
class YourActionInitialization : public G4VUserActionInitialization {

public:

    YourActionInitialization(std::string ofilename, const YourInputArgs * args);
    ~YourActionInitialization() override;

    void Build() const override;

    void BuildForMaster() const override;

  private:
    std::string _ofilename;
    const YourInputArgs * fInputArgs;
    G4MaterialScanner * ms;
};

#endif
