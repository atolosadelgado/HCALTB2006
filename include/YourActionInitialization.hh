#ifndef YOURACTIONINITIALIZATION_HH
#define YOURACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include <string>

class YourInputArgs;
class YourDetectorConstructor;
class G4MaterialScanner;

class YourActionInitialization : public G4VUserActionInitialization {

public:

    YourActionInitialization(const YourInputArgs * args, const YourDetectorConstructor * detector);
    ~YourActionInitialization() override;

    void Build() const override;

    void BuildForMaster() const override;

  private:
    const YourInputArgs * fInputArgs;
    const YourDetectorConstructor * fDetector;
    G4MaterialScanner * ms;
};

#endif
