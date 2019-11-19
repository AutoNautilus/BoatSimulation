#include "shapefil.h"


int main(){
  SHPHandle handle = SHPOpen("shapefile/110m_physical/ne_110m_land", "rb");
  SHPObject* o = SHPReadObject(handle, 0);
  return 0;
}
