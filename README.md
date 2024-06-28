# vtk-volume-visualisation

Download the dataset and put the dataset on desired location. Put the **cxx file** and **CMakeLists** text file at the same folder and same level as the dataset folder.

**Dataset**
1. Aneurism Dataset
2. Foot Dataset
3. Teapot Dataset
4. Wholefrog Dataset

**User Interaction**

['KeyInterpreter' is used to handle user keyboard inputs to adjust the rendering parameters such as iso-values and transfer functions.]

1. Key '**s**'

   Toggles the rendering modes which are between isosurface rendering and volume rendering.

3. Key '**0**' to '**9**'

   Allow user to select the point either the code or opacity transfer function but only
   works when using ray marching method. If the number of point selected is out of bound,
   error statement will be displayed.

5. Key '**/**'

   Toggles between ray step and transfer function editing mode when it is rendered in ray
   marching.

7. Key '**.**'

   Toggles between editing the points in color and opacity transfer functions.

9. Key '**+**' or '**=**'

   Can work with the key '/' (adjusting ray step) to increase the sample distance for ray
   casting or modify the selected transfer function point value.

11. Key '**-**' or '**_**'
   
    Also work with '/' (adjusting ray step) to decrease the sample distance for ray
    casting or modify the selected transfer function point value.
   
14. Key '**I**' and '**i**'
   
    Use during volume in iso surface method. 'I' can increase the iso-value used to
    extract the first iso surface by 100 while 'i' can decrease the iso-value used to
    extract the first iso surfaca by 100.

17. Key '**K**' and '**k**'
   
    Use during volume in iso surface method. 'K' can increase the iso-value used to
    extract the first iso surface by 100 while 'k' can decrease the iso-value used to
    extract the first iso surfaca by 100.


   
