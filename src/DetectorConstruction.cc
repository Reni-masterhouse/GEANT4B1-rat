//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "CADMesh.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 5*cm, env_sizeZ = 30*cm;
  // G4double env_sizeXY = 100*cm, env_sizeZ = 100*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Envelope
  //
  G4Box* solidEnv =
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size

  G4LogicalVolume* logicEnv =
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  // // Shape 1
  // G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_LUNG_ICRP");
  // G4ThreeVector pos1 = G4ThreeVector(30, 30, 30);

  // auto mesh1 = CADMesh::TessellatedMesh::FromOBJ("lungseg2.obj");
  // G4VSolid* solidShape1 = mesh1->GetSolid();

  // auto rot1 = new G4RotationMatrix();

  // rot1->rotateY(90. * deg);
  // rot1->rotateZ(-180. * deg);

  // G4LogicalVolume* logicShape1 =
  //   new G4LogicalVolume(solidShape1,         //its solid
  //                       shape1_mat,          //its material
  //                       "Shape1");           //its name

  // new G4PVPlacement(rot1,                       //no rotation
  //                   pos1,                    //at position
  //                   logicShape1,             //its logical volume
  //                   "Shape1",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

  // Shape 2
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4ThreeVector pos2 = G4ThreeVector(30, 30, 30);

  auto mesh2 = CADMesh::TessellatedMesh::FromOBJ("shape2.obj");
  G4VSolid* solidShape2 = mesh2->GetSolid();

  auto rot2 = new G4RotationMatrix();

  rot2->rotateY(90. * deg);
  rot2->rotateZ(-180. * deg);

  G4LogicalVolume* logicShape2 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

  new G4PVPlacement(rot2,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // // Shape 3
  // G4Material* shape3_mat = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
  // G4ThreeVector pos3 = G4ThreeVector(30, 30, 30);

  // auto mesh3 = CADMesh::TessellatedMesh::FromOBJ("liverseg.obj");
  // G4VSolid* solidShape3 = mesh3->GetSolid();

  // auto rot3 = new G4RotationMatrix();

  // rot3->rotateY(90. * deg);
  // rot3->rotateZ(-180. * deg);

  // G4LogicalVolume* logicShape3 =
  //   new G4LogicalVolume(solidShape3,         //its solid
  //                       shape3_mat,          //its material
  //                       "Shape3");           //its name

  // new G4PVPlacement(rot3,                       //no rotation
  //                   pos3,                    //at position
  //                   logicShape3,             //its logical volume
  //                   "Shape3",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

// // Shape 4
//   G4Material* shape4_mat = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
//   G4ThreeVector pos4 = G4ThreeVector(30, 30, 30);

//   auto mesh4 = CADMesh::TessellatedMesh::FromOBJ("spleen_fix.obj");
//   G4VSolid* solidShape4 = mesh4->GetSolid();

//   auto rot4 = new G4RotationMatrix();

//   rot4->rotateY(90. * deg);
//   rot4->rotateZ(-180. * deg);

//   G4LogicalVolume* logicShape4 =
//     new G4LogicalVolume(solidShape4,         //its solid
//                         shape4_mat,          //its material
//                         "Shape4");           //its name

//   new G4PVPlacement(rot4,                       //no rotation
//                     pos4,                    //at position
//                     logicShape4,             //its logical volume
//                     "Shape4",                //its name
//                     logicEnv,                //its mother  volume
//                     false,                   //no boolean operation
//                     0,                       //copy number
//                     checkOverlaps);          //overlaps checking

// // Shape 5
//   G4Material* shape5_mat = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
//   G4ThreeVector pos5 = G4ThreeVector(30, 30, 30);

//   auto mesh5 = CADMesh::TessellatedMesh::FromOBJ("heartseg.obj");
//   G4VSolid* solidShape5 = mesh5->GetSolid();

//   auto rot5 = new G4RotationMatrix();

//   rot5->rotateY(90. * deg);
//   rot5->rotateZ(-180. * deg);

//   G4LogicalVolume* logicShape5 =
//     new G4LogicalVolume(solidShape5,         //its solid
//                         shape5_mat,          //its material
//                         "Shape5");           //its name

//   new G4PVPlacement(rot5,                       //no rotation
//                     pos5,                    //at position
//                     logicShape5,             //its logical volume
//                     "Shape5",                //its name
//                     logicEnv,                //its mother  volume
//                     false,                   //no boolean operation
//                     0,                       //copy number
//                     checkOverlaps);          //overlaps checking

// // Shape 6
//   G4Material* shape6_mat = nist->FindOrBuildMaterial("G4_MUSCLE_SKELETAL_ICRP");
//   G4ThreeVector pos6 = G4ThreeVector(30, 30, 30);

//   auto mesh6 = CADMesh::TessellatedMesh::FromOBJ("rat_dilate_ugly.obj");
//   G4VSolid* solidShape6 = mesh6->GetSolid();

//   auto rot6 = new G4RotationMatrix();

//   rot6->rotateY(90. * deg);
//   rot6->rotateZ(-180. * deg);

//   G4LogicalVolume* logicShape6 =
//     new G4LogicalVolume(solidShape6,         //its solid
//                         shape6_mat,          //its material
//                         "Shape6");           //its name

//   new G4PVPlacement(rot6,                       //no rotation
//                     pos6,                    //at position
//                     logicShape6,             //its logical volume
//                     "Shape6",                //its name
//                     logicEnv,                //its mother  volume
//                     false,                   //no boolean operation
//                     0,                       //copy number
//                     checkOverlaps);          //overlaps checking


  // Shape 1
  //
  // G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  // G4ThreeVector pos1 = G4ThreeVector(0, 0, 0);

  // // Conical section shape
  // G4double shape1_rmina =  0.*cm, shape1_rmaxa = 2.*cm;
  // G4double shape1_rminb =  0.*cm, shape1_rmaxb = 4.*cm;
  // G4double shape1_hz = 3.*cm;
  // G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  // G4Cons* solidShape1 =
  //   new G4Cons("Shape1",
  //   shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb, shape1_hz,
  //   shape1_phimin, shape1_phimax);

  // G4LogicalVolume* logicShape1 =
  //   new G4LogicalVolume(solidShape1,         //its solid
  //                       shape1_mat,          //its material
  //                       "Shape1");           //its name

  // new G4PVPlacement(0,                       //no rotation
  //                   pos1,                    //at position
  //                   logicShape1,             //its logical volume
  //                   "Shape1",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

  
  // Shape 2
  //
  // G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_WATER");
  // G4ThreeVector pos2 = G4ThreeVector(0, 0, 0);

  // // Trapezoid shape
  // G4double shape2_dxa = 100*cm, shape2_dxb = 100*cm;
  // G4double shape2_dya = 100*cm, shape2_dyb = 100*cm;
  // G4double shape2_dz  = 100*cm;
  // G4Trd* solidShape2 =
  //   new G4Trd("Shape2",                      //its name
  //             0.5*shape2_dxa, 0.5*shape2_dxb,
  //             0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz); //its size

  // G4LogicalVolume* logicShape2 =
  //   new G4LogicalVolume(solidShape2,         //its solid
  //                       shape2_mat,          //its material
  //                       "Shape2");           //its name

  // new G4PVPlacement(0,                       //no rotation
  //                   pos2,                    //at position
  //                   logicShape2,             //its logical volume
  //                   "Shape2",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

  // // Set Shape2 as scoring volume
  // //
  fScoringVolume = logicShape1;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
