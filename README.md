Pepsi&Cola image detector

The image processing and label generation procedure commenced with the extraction of all images pertaining to Pepsi and Cola from the designated batch. These images were then organized into separate folders for training and testing purposes. Subsequently, a label generation function was implemented to assign labels to each picture, ensuring a deterministic division of images into training and testing sets based on predetermined rules, such as allocating the initial 100 images for training and the subsequent 230 images for testing. Labels were tagged either through folder numbers or within the file names, facilitating efficient data handling and storage in memory as well as in a CSV file.

Following the organization and labeling of images, accuracy computation entailed comparing the generated labels with the actual labels for each image. The count of correctly predicted labels, referred to as "ok" values, was tallied, and the total number of images was used to compute the accuracy percentage using the formula: Acuratccy = (ok / total). Post-evaluation, the accuracy of the prediction function was determined to be 80%.

To enhance prediction accuracy, a supplementary function was developed to generate labels based on dominant colors associated with Pepsi and Cola. This function focused on identifying dominant red and blue colors corresponding to Cola and Pepsi, respectively, and computing RGB values followed by Euclidean distance calculation to determine similarity. However, the accuracy of the enhanced prediction function was assessed to decrease to 43%. To address this, threshold binarization was implemented, resulting in an improved accuracy rate of 53%.

Error handling and visualization were facilitated through the creation of a dedicated folder to store incorrectly predicted images. Images with inaccurate predictions were automatically relocated to this folder using a specialized function. Initial assessment without threshold binarization revealed 103 misclassified images, which reduced to 82 following the implementation of threshold binarization, highlighting the efficacy of the approach in error reduction.

On the prediction method to see the result we used the confusion matrix. We add some related description to understand it better about what we see.
Confusion Matrix:

                   Actual
             |------------------|------------------|
  Predicted  |      Pepsi       |      Cola        |
  |----------|------------------|------------------|
  |   Pepsi  |   True Positive  |  False Negative  |
  |----------|------------------|------------------|
  |   Cola   |  False Positive  |   True Negative  |
  --------------------------------------------------

* True Positive (TP): The number of Pepsi images correctly classified as Pepsi.
* False Negative (FN): The number of Pepsi images incorrectly classified as Cola.
* False Positive (FP): The number of Cola images incorrectly classified as Pepsi.
* True Negative (TN): The number of Cola images correctly classified as Cola.

