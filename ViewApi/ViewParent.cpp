#include "ViewGroup.h"
#include "ViewParent.h"

void ViewParent::invalidateLayout(){
	if(invalidable)
		this->need_update_layout = 1;
}

void ViewParent::invalidableByChilds(bool inv){
	invalidable = inv;
}
