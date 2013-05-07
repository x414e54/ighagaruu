#include "AntCamera.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntCameraFollower::AntCameraFollower()
{
	_target=NULL;
}

//-----------------------------------------------------------------------------
// Updates the camera to be a certain position in the y direction and follow
// the target in the X and Z direction.
//-----------------------------------------------------------------------------
void AntCameraFollower::Update(float timeDelta)
{
	if (GetTarget()!=NULL) {
		_position.SetPosition(GetTarget()->_1,8.0f,GetTarget()->_3-0.1f);
	}
}
