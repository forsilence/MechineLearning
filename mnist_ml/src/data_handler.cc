#include "data_handler.hpp"
data_handler::data_handler()
{
  data_array = new std::vector<data *>;
  test_data = new std::vector<data *>;
  training_data = new std::vector<data *>;
  validation_data = new std::vector<data *>;
}
data_handler::~data_handler()
{
  // FREE Dynamicly Allocated Stuff
}

void data_handler::read_feature_vector(std::string path)
{
  uint32_t header[4];
  unsigned char bytes[4];
  printf("size of bytes type %lu.\n",sizeof(unsigned char));
  FILE *f = fopen(path.c_str() , "r");
  if (f){
    for (int i = 0; i < 4; ++i){
      if ( fread(bytes , sizeof(unsigned char), 4, f)){
        header[i] = convert_to_little_endian(bytes);
      }
    }
    printf("Done getting file headers.\n");
    int image_size = header[2] * header[3];
    printf("image size %d.\n",image_size);
    for (int i = 0; i < header[1]; ++i){
      data *d = new data();
      uint8_t element[1];
      printf("size of element type %lu.\n",sizeof(element));
      int read_out_value ;
      for ( int j = 0; j < image_size; ++j){
        if ( fread(element, sizeof(element), 1, f)){
          d->append_to_feature_vector(element[0]);
        }else{
          printf("Error Reading from feature File. i %d line %d read_out %d\n",i ,j,read_out_value);
          if (feof(f)){
            printf("end of file\n");
          }
          exit(1);
        }
      }
      data_array->push_back(d);
    }
    printf("Successfully read and stored %lu feature vectors.\n",data_array->size());
  }else{
    printf("Could not find file.\n");
    exit(1);
  }
}

void data_handler::read_feature_labels(std::string path)
{
  uint32_t header[4];
  unsigned char bytes[4];
  FILE *f = fopen(path.c_str() , "r");
  if (f){
    for (int i = 0; i < 4; ++i){
      if ( fread(bytes , sizeof(bytes), 1, f)){
        header[i] = convert_to_little_endian(bytes);
      }
    }
    printf("Done getting label file headers.\n");
    for (int i = 0; i < header[1]; ++i){
      uint8_t element[1];
      if (fread(element, sizeof(element), 1, f)){
        data_array->at(i)->set_label(element[0]);
      }else{
        printf("Error Reading from File.\n");
        exit(1);
      }
    }
    printf("Successfully read and stored %lu feature vectors.\n",data_array->size());
  }else{
    printf("Could not find file.\n");
    exit(1);
  }
}
void data_handler::split_data()
{
  std::unordered_set<int> used_indexes;
  int train_size = data_array->size() * TRAIN_SET_PERCENT;
  int test_size = data_array->size() * TEST_SET_PRECENT;
  int valid_size = data_array->size() * VALIDATION_PRECENT;

  // Training Data

  int  count = 0;
  while ( count < train_size){
    int rand_index = rand() % data_array->size() ;
    if ( used_indexes.find(rand_index) == used_indexes.end()) {
      training_data->push_back(data_array->at(rand_index));
    }
  }

  // Test Data
  count = 0;
  while ( count < test_size){
    int rand_index = rand() % data_array->size() ;
    if ( used_indexes.find(rand_index) == used_indexes.end()) {
      test_data->push_back(data_array->at(rand_index));
    }
  }

  // Validation Data
  count = 0;
  while ( count < valid_size){
    int rand_index = rand() % data_array->size() ;
    if ( used_indexes.find(rand_index) == used_indexes.end()) {
      validation_data->push_back(data_array->at(rand_index));
    }
  }

  printf("Training Data set size %lu.\n",training_data->size());
  printf("Test Data set size %lu.\n",test_data->size());
  printf("Validation Datas et size %lu.\n",validation_data->size());
}

void data_handler::count_classes()
{
  int count = 0;
  for (unsigned i = 0; i < data_array->size(); ++i) {
    if (class_map.find(data_array->at(i)->get_label()) == class_map.end()) {
      class_map[data_array->at(i)->get_label()] = count;
      data_array->at(i)->set_enumerated_label(count);
      ++count;
    }
  }
  num_classes = count ;
  printf("Successfully Extracted %d Unique Classes.\n", num_classes);
}

uint32_t data_handler::convert_to_little_endian( const unsigned char* bytes)
{
  return (uint32_t) ((bytes[0] << 24) |
                    (bytes[1] << 16) | 
                    (bytes[2] << 8 ) |
                    (bytes[3]));
}

std::vector<data *> * data_handler::get_training_data()
{
  return training_data;
}

std::vector<data *> * data_handler::get_test_data()
{
  return test_data;
}

std::vector<data *> * data_handler::get_validation_data()
{
  return validation_data;
}

int main()
{
  data_handler *dh = new data_handler();
  dh->read_feature_vector("./train-images-idx3-ubyte/train-images.idx3-ubyte");//
  dh->read_feature_labels("./train-labels-idx1-ubyte/train-labels.idx1-ubyte");
  dh->split_data();
  dh->count_classes();
  return 0;
}