# Overview 
Implementation of the Linear Hashing Data Structure. 

ADS_set is a container that stores elements identified by a key. Key is the type of the key and also the type of the element itself. This means that the entire stored element serves as the key. ADS_set is a simplification and generalization of the STL types std::set and std::unordered_set.

Each key can appear at most once in a container (set property). In other words, there cannot be two elements with the same key in any container. Unless specified otherwise for individual methods/functions, attempting to insert an already existing value is silently ignored. The same applies when trying to delete a non-existent value.

The container always stores copies of the elements passed to its methods. As a result, the container is responsible for the proper destruction of these elements.

A special Iterator class is implemented to iterate through the container. This iterator allows sequential access to the stored elements, enabling traversal without exposing the internal structure of the container. 
