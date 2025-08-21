#ifndef MYMETHOD_H
#define MYMETHODS_H

// Incluye clases necesarias de VTK para manejar eventos, visualización de imágenes y el slider
#include <vtkCommand.h>
#include <vtkImageViewer2.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation2D.h>

// Esta clase personalizada hereda de vtkCommand y se usará como callback para el slider
class SlicesSliderCallback : public vtkCommand {
    public:
        // Método estático para crear una nueva instancia del callback
        static SlicesSliderCallback* New();

        // Método que se ejecuta cuando el slider cambia de valor
        void Execute(vtkObject*, unsigned long, void*) override;

        // Puntero al visualizador de imágenes, necesario para actualizar la imagen mostrada
        vtkImageViewer2* viewer;
};

#endif //MYMETHODS_H
