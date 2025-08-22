#include "myMethods.h"

// Cabeceras de ITK para leer imágenes DICOM
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageToVTKImageFilter.h>

// Cabeceras de VTK para visualización
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

#include <iostream>
#include <string>

using std::cout;
using std::endl;

int main(int argc, char ** argv) {
    int minSlice, maxSlice, indexSlice;

    // Definimos el tipo de pixel y la dimensión de la imagen (3D)
    using pixelType = signed short;
    constexpr unsigned int dimension = 3;
    using imageType = itk::Image<pixelType, dimension>;

    // 🔍 ITK: Configuramos el lector de imágenes DICOM
    auto namesGenerator = itk::GDCMSeriesFileNames::New();
    namesGenerator->SetDirectory("/home/rv/ItkProjects/DicomVTKViewer/02ef8f31ea86a45cfce6eb297c274598/series-000001");

    auto reader = itk::ImageSeriesReader<imageType>::New();
    auto dicomIO = itk::GDCMImageIO::New();
    reader->SetImageIO(dicomIO);
    reader->SetFileNames(namesGenerator->GetInputFileNames());
    reader->Update(); // Carga la imagen en memoria

    // 🔄 ITK → VTK: Convertimos la imagen ITK a formato VTK
    using connectorType = itk::ImageToVTKImageFilter<imageType>;
    auto connector = connectorType::New();
    connector->SetInput(reader->GetOutput());
    connector->Update();

    // 🖥️ VTK: Configuramos el visualizador de imágenes
    auto viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputData(connector->GetOutput());

    // Obtenemos el rango de slices disponibles
    minSlice = viewer->GetSliceMin();
    maxSlice = viewer->GetSliceMax();
    cout << "Slice Range: " << minSlice << " to " << maxSlice << endl;

    // Establecemos el slice inicial (por defecto es 0)
    viewer->SetSlice(indexSlice);

    // Creamos el interactor para manejar eventos del usuario (mouse, teclado)
    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    viewer->SetupInteractor(interactor);
    viewer->Render();
    viewer->GetRenderer()->ResetCamera();

    // 🎚️ Interfaz: Creamos el slider para navegar entre slices
    auto sliderRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
    sliderRep->SetMinimumValue(minSlice);  // Valor mínimo del slider
    sliderRep->SetMaximumValue(maxSlice);  // Valor máximo del slider
    sliderRep->SetValue(minSlice);         // Valor inicial
    sliderRep->SetTitleText("Slice");      // Texto del slider

    // Posicionamos el slider en la ventana (coordenadas normalizadas)
    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
    sliderRep->GetPoint1Coordinate()->SetValue(0.1, 0.1);
    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
    sliderRep->GetPoint2Coordinate()->SetValue(0.9, 0.1);

    // Ajustes visuales del slider
    sliderRep->SetSliderLength(0.02);
    sliderRep->SetSliderWidth(0.03);
    sliderRep->SetEndCapLength(0.01);
    sliderRep->SetTubeWidth(0.005);
    sliderRep->SetLabelFormat("%3.0lf");
    sliderRep->SetTitleHeight(0.02);
    sliderRep->SetLabelHeight(0.02);

    // Creamos el widget del slider y lo conectamos al interactor
    auto sliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
    sliderWidget->SetInteractor(interactor);
    sliderWidget->SetRepresentation(sliderRep);
    sliderWidget->SetAnimationModeToAnimate(); // Suaviza el movimiento
    sliderWidget->EnabledOn(); // Activa el widget

    // 🔁 Conectamos el slider al callback que actualiza el slice mostrado
    auto callback = vtkSmartPointer<SlicesSliderCallback>::New();
    callback->viewer = viewer; // Le pasamos el viewer al callback
    sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback.GetPointer());

    // 🧩 Inicia el bucle de eventos (renderizado + interacción)
    interactor->Start();

    return EXIT_SUCCESS;
}
