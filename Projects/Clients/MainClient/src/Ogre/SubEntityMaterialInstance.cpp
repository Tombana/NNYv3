#include "SubEntityMaterialInstance.h"

SubEntityMaterialInstance::SubEntityMaterialInstance (SubEntity *se) : MaterialInstance () {
  mSubEntity = se;
      
  initOriginalMaterial ();
}

SubEntityMaterialInstance::~SubEntityMaterialInstance () {
  // Reset to the original material
  mSubEntity->setMaterialName (mOriginalMat->getName ());
}

void SubEntityMaterialInstance::setMaterialName (String name) {
  clearCopyMaterial ();
      
  mSubEntity->setMaterialName (name);
      
  initOriginalMaterial ();
      
  setTransparency (mCurrentTransparency);
}

void SubEntityMaterialInstance::setTransparency (Real transparency) {
  MaterialInstance::setTransparency (transparency);
        
  if (mCurrentTransparency > 0.0f) {
    mSubEntity->setMaterialName (mCopyMat->getName ());
  }
  else {
    mSubEntity->setMaterialName (mOriginalMat->getName ());
  }
}

void SubEntityMaterialInstance::initOriginalMaterial () {
  mOriginalMat = MaterialManager::getSingleton ().getByName (mSubEntity->getMaterialName ());
}
