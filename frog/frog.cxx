#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkStructuredPointsReader.h"
#include "vtkMarchingCubes.h"
#include "vtkRecursiveDividingCubes.h"
#include "vtkScalarBarWidget.h"
#include "vtkScalarBarActor.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
// #include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
// #include "vtkVolumeRayCastMIPFunction.h"

#include <iostream>
using namespace std;

int mode = 0;
int data = 0;
bool rayStep = true;
bool changeColorOpac = true;

void setColorOpacity(vtkActor* contour, vtkContourFilter* contourExtractor, vtkColorTransferFunction* colorTransferFunction, vtkPiecewiseFunction* opacityTransferFunction) {
	contour->GetProperty()->SetColor(colorTransferFunction->GetRedValue(contourExtractor->GetValue(0)), colorTransferFunction->GetGreenValue(contourExtractor->GetValue(0)), colorTransferFunction->GetBlueValue(contourExtractor->GetValue(0)));
	contour->GetProperty()->SetOpacity(opacityTransferFunction->GetValue(contourExtractor->GetValue(0)));
}

class KeyInterpreter : public vtkCommand {
public:
	// Allocator
	static KeyInterpreter* New(void) { return new KeyInterpreter; }

	// Internal data.  This needs to be set immediately after allocation.
	vtkFixedPointVolumeRayCastMapper* map;
	vtkContourFilter* contourExtractor1;
	vtkContourFilter* contourExtractor2;
	vtkRenderer* renderer;
	vtkActor* outline;
	vtkActor* contour1;
	vtkActor* contour2;
	vtkVolume* volume;
	vtkColorTransferFunction* colorTransferFunction;
	vtkPiecewiseFunction* opacityTransferFunction;
	vtkColorTransferFunction* volumeColorTransferFunction;
	vtkPiecewiseFunction* volumeOpacityTransferFunction;

	int maxNumVolumeColor;
	int maxNumVolumeOpac;

	void assignMaxNum() {
		maxNumVolumeColor = volumeColorTransferFunction->GetSize();
		maxNumVolumeOpac = volumeOpacityTransferFunction->GetSize();
	}

	int selectedPoint = -1;

	// Variables for iso-value and ray step size
	double stepSize;
	double isoValue1 = 1500;
	double isoValue2 = 2750;

	KeyInterpreter() {
		stepSize = 1.0; // Initial step size
	}

	// Called upon the registered event (i.e., a key press)
	void Execute(vtkObject* caller, unsigned long eventId, void* callData) {
		int point;
		vtkRenderWindowInteractor* iren =
			reinterpret_cast<vtkRenderWindowInteractor*>(caller);
		switch (iren->GetKeyCode()) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			point = int(iren->GetKeyCode()) - 48;
			if (changeColorOpac) {
				if (point < maxNumVolumeColor) {
					selectedPoint = point;
				}
				else {
					cout << "Number of point selected out of bound.\n" << endl;
				}
			}
			else {
				if (point < maxNumVolumeOpac) {
					selectedPoint = point;
				}
				else {
					cout << "Number of point selected out of bound.\n" << endl;
				}
			}
			break;
		case 'r':
		case 'R':
			rayStep = !rayStep;
			if (rayStep)
				printf("rayStep is true\n");
			else
				printf("rayStep is false\n");
			break;
		case 'o':
		case 'O':
			changeColorOpac = !changeColorOpac;
			if (changeColorOpac)
				printf("changeColorOpac is true\n");
			else
				printf("changeColorOpac is false\n");
			break;
		case '+':
		case '=':
			if (mode == 0) {
				if (rayStep) {
					stepSize = map->GetSampleDistance();
					stepSize += 1;
					map->SetSampleDistance(stepSize);
					cout << "step size increased for ray marching to " << stepSize << endl;
				}
				else {
					if (changeColorOpac) {
						if (selectedPoint >= 0 && selectedPoint < maxNumVolumeColor) {
							double pos[6];
							volumeColorTransferFunction->GetNodeValue(selectedPoint, pos);
							pos[0] += 50;
							volumeColorTransferFunction->SetNodeValue(selectedPoint, pos);
							printf("increase the position of selected control point in the color transfer function for ray marching\n");
						}
					}
					else {
						if (selectedPoint >= 0 && selectedPoint < maxNumVolumeOpac) {
							double pos[6];
							volumeOpacityTransferFunction->GetNodeValue(selectedPoint, pos);
							pos[0] += 50;
							volumeOpacityTransferFunction->SetNodeValue(selectedPoint, pos);
							printf("increase the position of selected control point in the opacity transfer function for ray marching\n");
						}
					}
				}
			}
			break;
		case '-':
		case '_':
			if (mode == 0) {
				if (rayStep) {
					stepSize = map->GetSampleDistance();
					if (stepSize > 1) {
						stepSize -= 1;
						map->SetSampleDistance(stepSize);
						cout << "step size decreased for ray marching to " << stepSize << endl;
					}
				}
				else {
					if (changeColorOpac) {
						if (selectedPoint >= 0 && selectedPoint < maxNumVolumeColor) {
							double pos[6];
							volumeColorTransferFunction->GetNodeValue(selectedPoint, pos);
							pos[0] -= 50;
							volumeColorTransferFunction->SetNodeValue(selectedPoint, pos);
							printf("decrease the position of selected control point in the color transfer function for ray marching\n");
						}
					}
					else {
						if (selectedPoint >= 0 && selectedPoint < maxNumVolumeOpac) {
							double pos[6];
							volumeOpacityTransferFunction->GetNodeValue(selectedPoint, pos);
							pos[0] -= 50;
							volumeOpacityTransferFunction->SetNodeValue(selectedPoint, pos);
							printf("decrease the position of selected control point in the opacity transfer function for ray marching\n");
						}
					}
				}
			}
			break;
		case 'I':
			if (mode == 1) {
				isoValue1 += 100.0; // Increase iso-value for contourExtractor1
				contourExtractor1->SetValue(0, isoValue1);
				setColorOpacity(contour1, contourExtractor1, colorTransferFunction, opacityTransferFunction);
				printf("increased the isoValue1 to ");
				cout << isoValue1 << endl;
			}
			break;
		case 'i':
			if (mode == 1) {
				isoValue1 -= 100.0; // Decrease iso-value for contourExtractor1
				if (isoValue1 < 0.0) isoValue1 = 0.0; // Ensure it doesn't go below 0
				contourExtractor1->SetValue(0, isoValue1);
				setColorOpacity(contour1, contourExtractor1, colorTransferFunction, opacityTransferFunction);
				printf("decreased the isoValue1 to ");
				cout << isoValue1 << endl;
			}
			break;
		case 'K':
			if (mode == 1) {
				isoValue2 += 100.0; // Increase iso-value for contourExtractor2
				contourExtractor2->SetValue(0, isoValue2);
				setColorOpacity(contour2, contourExtractor2, colorTransferFunction, opacityTransferFunction);
				printf("increased the isoValue2 to ");
				cout << isoValue2 << endl;
			}
			break;
		case 'k':
			if (mode == 1) {
				isoValue2 -= 100.0; // Decrease iso-value for contourExtractor2
				if (isoValue2 < 0.0) isoValue2 = 0.0; // Ensure it doesn't go below 0
				contourExtractor2->SetValue(0, isoValue2);
				setColorOpacity(contour2, contourExtractor2, colorTransferFunction, opacityTransferFunction);
				printf("decreased the isoValue2 to ");
				cout << isoValue2 << endl;
			}
			break;
			// Interchanged between isosurface/ray marching
		case 's':
		case 'S':
			renderer->RemoveAllViewProps();
			renderer->AddActor(outline);
			vtkScalarBarWidget* scalarWidget = vtkScalarBarWidget::New();
			scalarWidget->SetInteractor(iren);
			scalarWidget->GetScalarBarActor()->SetTitle("Transfer Function");
			if (mode == 0) {
				renderer->AddActor(contour1);
				renderer->AddActor(contour2);
				scalarWidget->GetScalarBarActor()->SetLookupTable(colorTransferFunction);
				mode = 1;
				printf("changed to isosurface rendering\n");
			}
			else if (mode == 1) {
				renderer->AddVolume(volume);
				scalarWidget->GetScalarBarActor()->SetLookupTable(volumeColorTransferFunction);
				mode = 0;
				printf("changed to ray marching\n");
			}
			scalarWidget->EnabledOn();
			break;
		}
		iren->Render();
	}
};

int main(int argc, char** argv)
{
	// Create the renderer, the render window, and the interactor. The renderer
	// draws into the render window, the interactor enables mouse- and 
	// keyboard-based interaction with the data within the render window.
	vtkRenderer* aRenderer = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->SetStereoTypeToInterlaced();
	renWin->AddRenderer(aRenderer);
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// For frog2ci dataset
//	/*
	double isoValue1 = 5000;
	double isoValue2 = 20000;
	//	*/

	// vtkVolumeReader16 reads in the head CT data set.  
	vtkVolume16Reader* reader = vtkVolume16Reader::New();
	
	reader->SetDataDimensions(256, 256);
	reader->SetImageRange(1, 44);
	reader->SetDataByteOrderToBigEndian();
	reader->SetFilePrefix("../data/frog2ci");
	reader->SetDataSpacing(1.0, 1.0, 2.2);
	
	// This part creates the colorMap function to the isosurface rendering.
	vtkPiecewiseFunction* isoOpacityTransferFunction = vtkPiecewiseFunction::New();
	
	// For frog2ci dataset
	isoOpacityTransferFunction->AddPoint(0, 0.0);
	isoOpacityTransferFunction->AddPoint(4000, 0.0);
	isoOpacityTransferFunction->AddPoint(5000, 0.6);
	isoOpacityTransferFunction->AddPoint(7000, 0.8);
	isoOpacityTransferFunction->AddPoint(15000, 0.9);

	vtkColorTransferFunction* isoColorTransferFunction = vtkColorTransferFunction::New();

	// For frog2ci dataset
	isoColorTransferFunction->AddRGBPoint(3.0, .0, 0.0, 0.0);
	isoColorTransferFunction->AddRGBPoint(5000.0, 1.0, 1.0, 0.5);
	isoColorTransferFunction->AddRGBPoint(6000.0, 1.0, 1.0, 0.5);
	isoColorTransferFunction->AddRGBPoint(8000.0, 0.94, 0.5, 0.5);
	isoColorTransferFunction->AddRGBPoint(10000.0, 0.94, 0.5, 0.5);

	// This part creates the colorMap function to the volume rendering.
	vtkPiecewiseFunction* opacityTransferFunction = vtkPiecewiseFunction::New();
	vtkColorTransferFunction* colorTransferFunction = vtkColorTransferFunction::New();
	
	// For frog2ci dataset 
	opacityTransferFunction->AddPoint(20, 0.0);
	opacityTransferFunction->AddPoint(4000, 0.0);
	opacityTransferFunction->AddPoint(5000, 0.3);
	opacityTransferFunction->AddPoint(7000, 0.5);
	opacityTransferFunction->AddPoint(13000, 0.9);
	opacityTransferFunction->AddPoint(20000, 0.9);

	colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(4000, 1.0, 0.6, 0.0);
	colorTransferFunction->AddRGBPoint(5000, 1.0, 0.6, 0.0);
	colorTransferFunction->AddRGBPoint(7000, 1.0, 0.6, 0.0);
	colorTransferFunction->AddRGBPoint(13000, 0.9, 0.9, 0.9);
	colorTransferFunction->AddRGBPoint(20000, 0.9, 0.9, 0.9);

	// Starting of isosurface
	//
	// This next section creates two contours for the density data.  A
	// vtkContourFilter object is created that takes the input data from
	// the reader.																		
	vtkContourFilter* contourExtractor = vtkContourFilter::New();
	contourExtractor->SetInputConnection(reader->GetOutputPort());
	contourExtractor->SetValue(0, isoValue1);

	vtkContourFilter* contourExtractor2 = vtkContourFilter::New();
	contourExtractor2->SetInputConnection(reader->GetOutputPort());
	contourExtractor2->SetValue(0, isoValue2);

	// This section creates the polygon normals for the contour surfaces
	//  and creates the mapper that takes in the newly normalized surfaces
	vtkPolyDataNormals* contourNormals = vtkPolyDataNormals::New();
	contourNormals->SetInputConnection(contourExtractor->GetOutputPort());
	contourNormals->SetFeatureAngle(60.0);
	vtkPolyDataMapper* contourMapper = vtkPolyDataMapper::New();
	contourMapper->SetInputConnection(contourNormals->GetOutputPort());
	contourMapper->ScalarVisibilityOff();
	vtkPolyDataNormals* contourNormals2 = vtkPolyDataNormals::New();
	contourNormals2->SetInputConnection(contourExtractor2->GetOutputPort());
	contourNormals2->SetFeatureAngle(60.0);
	vtkPolyDataMapper* contourMapper2 = vtkPolyDataMapper::New();
	contourMapper2->SetInputConnection(contourNormals2->GetOutputPort());
	contourMapper2->ScalarVisibilityOff();

	// This section sets up the Actor that takes the contour
	// This is where you can set the color and opacity of the two contours
	vtkActor* contour = vtkActor::New();
	contour->SetMapper(contourMapper);
	setColorOpacity(contour, contourExtractor, isoColorTransferFunction, isoOpacityTransferFunction);
	vtkActor* contour2 = vtkActor::New();
	contour2->SetMapper(contourMapper2);
	setColorOpacity(contour2, contourExtractor2, isoColorTransferFunction, isoOpacityTransferFunction);

	// End of isosurface

	// The property describes how the data will look
	vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	// vtkVolumeRay Functions
	// vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
	//	vtkVolumeRayCastMIPFunction* compositeFunction = vtkVolumeRayCastMIPFunction::New();

	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(reader->GetOutputPort());

	// Set the sampling rate
	volumeMapper->SetSampleDistance(1.0);

	// The volume holds the mapper and the property and
	// can be used to position/orient the volume
	vtkVolume* volume = vtkVolume::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// An outline provides context around the data.
	vtkOutlineFilter* outlineData = vtkOutlineFilter::New();
	outlineData->SetInputConnection(reader->GetOutputPort());
	vtkPolyDataMapper* mapOutline = vtkPolyDataMapper::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());
	vtkActor* outline = vtkActor::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	// It is convenient to create an initial view of the data. The FocalPoint
	// and Position form a vector direction. Later on (ResetCamera() method)
	// this vector is used to position the camera to look at the data in
	// this direction.
	vtkCamera* aCamera = vtkCamera::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();

	// Actors are added to the renderer. An initial camera view is created.
	// The Dolly() method moves the camera towards the FocalPoint,
	// thereby enlarging the image.
	aRenderer->AddActor(outline);
	aRenderer->AddVolume(volume);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aCamera->Dolly(1.5);

	// Set a background color for the renderer and set the size of the
	// render window (expressed in pixels).
	aRenderer->SetBackground(1, 1, 1);
	renWin->SetSize(800, 600);

	// Note that when camera movement occurs (as it does in the Dolly()
	// method), the clipping planes often need adjusting. Clipping planes
	// consist of two planes: near and far along the view direction. The 
	// near plane clips out objects in front of the plane; the far plane
	// clips out objects behind the plane. This way only what is drawn
	// between the planes is actually rendered.
	aRenderer->ResetCameraClippingRange();

	vtkScalarBarWidget* scalarWidget = vtkScalarBarWidget::New();
	scalarWidget->SetInteractor(iren);
	scalarWidget->GetScalarBarActor()->SetTitle("Transfer Function");
	scalarWidget->GetScalarBarActor()->SetLookupTable(colorTransferFunction);

	KeyInterpreter* key = KeyInterpreter::New();
	key->renderer = aRenderer;
	key->map = volumeMapper;
	key->contourExtractor1 = contourExtractor;
	key->contourExtractor2 = contourExtractor2;
	key->outline = outline;
	key->contour1 = contour;
	key->contour2 = contour2;
	key->volume = volume;
	key->colorTransferFunction = isoColorTransferFunction;
	key->opacityTransferFunction = isoOpacityTransferFunction;
	key->volumeColorTransferFunction = colorTransferFunction;
	key->volumeOpacityTransferFunction = opacityTransferFunction;
	key->isoValue1 = isoValue1;
	key->isoValue2 = isoValue2;
	key->assignMaxNum();
	iren->AddObserver(vtkCommand::KeyPressEvent, key);

	// Initialize the event loop and then start it.
	iren->Initialize();
	renWin->SetWindowName("Simple Volume Renderer");
	renWin->Render();
	scalarWidget->EnabledOn();
	iren->Start();

	// It is important to delete all objects created previously to prevent
  // memory leaks. In this case, since the program is on its way to
  // exiting, it is not so important. But in applications it is
  // essential.
	reader->Delete();
	opacityTransferFunction->Delete();
	colorTransferFunction->Delete();
	contourExtractor->Delete();
	contourExtractor2->Delete();
	contourNormals->Delete();
	contourNormals2->Delete();
	contourMapper->Delete();
	contourMapper2->Delete();
	contour->Delete();
	contour2->Delete();
	outlineData->Delete();
	mapOutline->Delete();
	outline->Delete();
	aCamera->Delete();
	iren->Delete();
	renWin->Delete();
	aRenderer->Delete();
	key->Delete();
	scalarWidget->Delete();

	return 0;
}
