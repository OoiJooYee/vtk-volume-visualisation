# vtk-volume-visualisation

Download the dataset and put the dataset on desired location. Put the **cxx file** and **CMakeLists** text file at the same folder and same level as the dataset folder.

**Dataset**
1. CT Head Dataset
2. Foot Dataset
3. Teapot Dataset
4. Wholefrog Dataset


**Important Surfaces**
1. CT Head Dataset

   The CT head dataset has two interesting surfaces. The first interesting surface is the skull of the head, the second interesting surface is the brain inside the skull. 

2. Foot Dataset

   The foot dataset has three interesting surfaces. The first interesting surface is the skin of the foot, the second interesting surface is the flesh of the foot and the third interesting surface is the bones of the foot. 

3. Teapot Dataset

   The teapot dataset has four interesting surfaces. The first surface is the table and the tray below the teapot, second surface is the body of the teapot, the third surface is the body of the container inside the teapot and the fourth surface is the lobster inside the container.

4. Wholefrog Dataset

   The whole frog dataset has three interesting surfaces. The first surface is the skin of the frog, the second surface is the bones of the frog and the third surface is the organs of the frog.



**Discussion on why a larger number of surfaces make the image hard to comprehend**

Rendering a large number of surfaces in VTK can make the image difficult to understand for several reasons. The inner surfaces can become hidden by the outer surfaces, leading to occlusion and making it hard to see the full 3D shape. Having too many surfaces in a single view creates a cluttered image, making it challenging to distinguish individual surfaces and understand their spatial relationships. Depth perception also becomes more difficult with many overlapping surfaces, as depth cues like occlusion and lighting are less effective. Additionally, rendering a high number of surfaces requires significant processing power, which can result in performance issues like slower frame rates, hindering smooth exploration of the image. Finally, the viewer's attention gets divided across the entire volume, making it harder to focus on specific areas of interest. 


**User Interaction**

['KeyInterpreter' is used to handle user keyboard inputs to adjust the rendering parameters such as iso-values and transfer functions.]

1. Key '**s**' or **'S'**

   Toggles the rendering modes which are between isosurface rendering and ray marching rendering.

3. Key '**0**' to '**9**'

   Allow user to select the point either the code or opacity transfer function but only
   works when using ray marching method. If the number of point selected is out of bound,
   error statement will be displayed.

5. Key '**r**' or **'R'**

   Toggles between ray step and transfer function editing mode when it is rendered in ray
   marching.

7. Key '**o**' or **'O'**

   Toggles between editing the points in color and opacity transfer functions.

9. Key '**+**' or '**=**'

   Can work with the key 'r'/'R' (adjusting ray step) to increase the step size for ray
   marching or modify the color/opacity transfer function point value.

11. Key '**-**' or '**_**'
   
    Also work with 'r'/'R' (adjusting ray step) to decrease the step size for ray
    marching or modify the color/opacity transfer function point value.
   
14. Key '**I**' and '**i**'
   
    Use during isosurface rendering. 'I' can increase the iso-value used to
    extract the first iso surface by 100 while 'i' can decrease the iso-value used to
    extract the first iso surfaca by 100.

17. Key '**K**' and '**k**'
   
    Use during isosurface rendering. 'K' can increase the iso-value used to
    extract the first iso surface by 100 while 'k' can decrease the iso-value used to
    extract the first iso surfaca by 100.


   
