#ifndef __ALLXXX_H__
#define __ALLXXX_H__

//#define STATIC_CREATE( T ) \
//template<typename ...PTS> \
//inline static T* Create( PTS&& ...ps ) \
//{ \
//    auto rtv = new T( std::forward<PTS>( ps )... ); \
//    rtv->AutoRelease(); \
//    return rtv; \
//}

#include "Ref.h"
#include "Node.h"
#include "TouchEvent.h"
#include "Input.h"
#include "Window.h"
#include "Audio.h"
#include "Box.h"
#include "Scene.h"
#include "Game.h"
#include "Looper.h"
#include "G.h"
#include "CD.h"

#endif