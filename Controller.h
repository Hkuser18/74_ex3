/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
#include<memory>
#include "View.h"

class Controller {
public:	
	Controller();
	~Controller();

	// creates View object, runs the program by accepting user commands, then destroys View object
	void run();

private:
	std::shared_ptr<View> view_ptr;
};