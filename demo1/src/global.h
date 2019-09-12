//// BGR vs HSV
#define HSV 1

//// HSV
// Thresholding for binary image
#define RED_LASER_HSV_HUE_LOW       140
#define RED_LASER_HSV_SAT_LOW       30
#define RED_LASER_HSV_VAL_LOW       190
#define RED_LASER_HSV_HUE_HIGH      195
#define RED_LASER_HSV_SAT_HIGH      163
#define RED_LASER_HSV_VAL_HIGH      240

#define GREEN_LASER_HSV_HUE_LOW     30
#define GREEN_LASER_HSV_SAT_LOW     50
#define GREEN_LASER_HSV_VAL_LOW     100
#define GREEN_LASER_HSV_HUE_HIGH    65
#define GREEN_LASER_HSV_SAT_HIGH    210
#define GREEN_LASER_HSV_VAL_HIGH    240

// Expected Values Scoring
#define BLOB_RED_LASER_HSV_HUE      156
#define BLOB_RED_LASER_HSV_SAT      42
#define BLOB_RED_LASER_HSV_VAL      222
#define EDGE_RED_LASER_HSV_HUE      159
#define EDGE_RED_LASER_HSV_SAT      65
#define EDGE_RED_LASER_HSV_VAL      200
#define CORE_RED_LASER_HSV_HUE      153
#define CORE_RED_LASER_HSV_SAT      11
#define CORE_RED_LASER_HSV_VAL      252

#define BLOB_GREEN_LASER_HSV_HUE    70
#define BLOB_GREEN_LASER_HSV_SAT    122
#define BLOB_GREEN_LASER_HSV_VAL    168
#define EDGE_GREEN_LASER_HSV_HUE    130
#define EDGE_GREEN_LASER_HSV_SAT    100
#define EDGE_GREEN_LASER_HSV_VAL    100
#define CORE_GREEN_LASER_HSV_HUE    255
#define CORE_GREEN_LASER_HSV_SAT    255
#define CORE_GREEN_LASER_HSV_VAL    255

//// BGR
// Thresholding for binary image
#define RED_LASER_BGR_BLUE_LOW      0
#define RED_LASER_BGR_GREEN_LOW     0
#define RED_LASER_BGR_RED_LOW       180
#define RED_LASER_BGR_BLUE_HIGH     182
#define RED_LASER_BGR_GREEN_HIGH    182
#define RED_LASER_BGR_RED_HIGH      255

#define GREEN_LASER_BGR_BLUE_LOW    0
#define GREEN_LASER_BGR_GREEN_LOW   0
#define GREEN_LASER_BGR_RED_LOW     0
#define GREEN_LASER_BGR_BLUE_HIGH   0
#define GREEN_LASER_BGR_GREEN_HIGH  0
#define GREEN_LASER_BGR_RED_HIGH    0

// Expected Values Scoring
#define BLOB_RED_LASER_BGR_BLUE   174
#define BLOB_RED_LASER_BGR_GREEN  162
#define BLOB_RED_LASER_BGR_RED    223
#define EDGE_RED_LASER_BGR_BLUE   180
#define EDGE_RED_LASER_BGR_GREEN  180
#define EDGE_RED_LASER_BGR_RED    255
#define CORE_RED_LASER_BGR_BLUE   255
#define CORE_RED_LASER_BGR_GREEN  255
#define CORE_RED_LASER_BGR_RED    255

#define BLOB_GREEN_LASER_BGR_BLUE   0  
#define BLOB_GREEN_LASER_BGR_GREEN  0  
#define BLOB_GREEN_LASER_BGR_RED    0  
#define EDGE_GREEN_LASER_BGR_BLUE   0  
#define EDGE_GREEN_LASER_BGR_GREEN  0  
#define EDGE_GREEN_LASER_BGR_RED    0  
#define CORE_GREEN_LASER_BGR_BLUE   0  
#define CORE_GREEN_LASER_BGR_GREEN  0  
#define CORE_GREEN_LASER_BGR_RED    0  

//// Camera
#define CAMERA_NUMBER      2 // list available cameras with 'v4l2-ctl --list-devices'
#define MIN_ROW            0 
#define MIN_COL            0
#define MAX_ROW          480
#define MAX_COL          640
#define FPS               30
#define BUFFERSIZE         1
#define FRAME_FIFO_SIZE   10     // How many frames do we want in our buffer?


//// Blob Detection
#define MAX_MINESWEEP_OFFSET 10 // will check (x +/- offset, y +/- offset) for neighbor pixels
#define NEIGHBOR_COUNT        8 // # of neighbor pixels required to constitute as a "core" pixel (any less will be edge)


//// Blob Selection
// Filters
// HSV
#define FILTER_RED_LASER_BLOB_HSV_HUE     1
#define FILTER_RED_LASER_BLOB_HSV_SAT     0
#define FILTER_RED_LASER_BLOB_HSV_VAL     1
#define FILTER_RED_LASER_EDGE_HSV_HUE     1
#define FILTER_RED_LASER_EDGE_HSV_SAT     0
#define FILTER_RED_LASER_EDGE_HSV_VAL     1
#define FILTER_RED_LASER_CORE_HSV_HUE     1
#define FILTER_RED_LASER_CORE_HSV_SAT     0
#define FILTER_RED_LASER_CORE_HSV_VAL     1

#define FILTER_GREEN_LASER_BLOB_HSV_HUE   1
#define FILTER_GREEN_LASER_BLOB_HSV_SAT   0
#define FILTER_GREEN_LASER_BLOB_HSV_VAL   1
#define FILTER_GREEN_LASER_EDGE_HSV_HUE   1
#define FILTER_GREEN_LASER_EDGE_HSV_SAT   0
#define FILTER_GREEN_LASER_EDGE_HSV_VAL   1
#define FILTER_GREEN_LASER_CORE_HSV_HUE   1
#define FILTER_GREEN_LASER_CORE_HSV_SAT   0
#define FILTER_GREEN_LASER_CORE_HSV_VAL   1

#define FILTER_RED_LASER_HSV_SIZE         0
#define FILTER_GREEN_LASER_HSV_SIZE       0

// BGR
#define FILTER_RED_LASER_BLOB_BGR_BLUE    0
#define FILTER_RED_LASER_BLOB_BGR_GREEN   0
#define FILTER_RED_LASER_BLOB_BGR_RED     1
#define FILTER_RED_LASER_EDGE_BGR_BLUE    0
#define FILTER_RED_LASER_EDGE_BGR_GREEN   0
#define FILTER_RED_LASER_EDGE_BGR_RED     1
#define FILTER_RED_LASER_CORE_BGR_BLUE    1
#define FILTER_RED_LASER_CORE_BGR_GREEN   1
#define FILTER_RED_LASER_CORE_BGR_RED     1

#define FILTER_GREEN_LASER_BLOB_BGR_BLUE  0
#define FILTER_GREEN_LASER_BLOB_BGR_GREEN 0
#define FILTER_GREEN_LASER_BLOB_BGR_RED   1
#define FILTER_GREEN_LASER_EDGE_BGR_BLUE  0
#define FILTER_GREEN_LASER_EDGE_BGR_GREEN 0
#define FILTER_GREEN_LASER_EDGE_BGR_RED   1
#define FILTER_GREEN_LASER_CORE_BGR_BLUE  1
#define FILTER_GREEN_LASER_CORE_BGR_GREEN 1
#define FILTER_GREEN_LASER_CORE_BGR_RED   1

#define FILTER_RED_LASER_BGR_SIZE         0
#define FILTER_GREEN_LASER_BGR_SIZE       0

// Weights / Importance
#ifdef HSV
#define BLOB_BLUE_WEIGHT      2 // HSV not BGR (TODO: make this less confusing)
#define BLOB_GREEN_WEIGHT     1 // HSV not BGR (TODO: make this less confusing)
#define BLOB_RED_WEIGHT       1 // HSV not BGR (TODO: make this less confusing)
#define EDGE_BLUE_WEIGHT      2 // HSV not BGR (TODO: make this less confusing)
#define EDGE_GREEN_WEIGHT     1 // HSV not BGR (TODO: make this less confusing)
#define EDGE_RED_WEIGHT       1 // HSV not BGR (TODO: make this less confusing)
#define CORE_BLUE_WEIGHT      1 // HSV not BGR (TODO: make this less confusing)
#define CORE_GREEN_WEIGHT     1 // HSV not BGR (TODO: make this less confusing)
#define CORE_RED_WEIGHT       1 // HSV not BGR (TODO: make this less confusing)
#define SCORE_CUTOFF       1000
#else
#define BLOB_BLUE_WEIGHT      1 
#define BLOB_GREEN_WEIGHT     1 
#define BLOB_RED_WEIGHT       2 
#define EDGE_BLUE_WEIGHT      1 
#define EDGE_GREEN_WEIGHT     1 
#define EDGE_RED_WEIGHT       4 
#define CORE_BLUE_WEIGHT      1 
#define CORE_GREEN_WEIGHT     1 
#define CORE_RED_WEIGHT       1 
#define SCORE_CUTOFF       1000
#endif

#define SIZE_IDEAL            4 // #Pixels in blob
#define SIZE_WEIGHT           0 // 


//// Pong
#define BALL_RED           150
#define BALL_GREEN          50
#define BALL_BLUE          150
#define PADDLE_LEFT_RED    100 
#define PADDLE_LEFT_GREEN    0 
#define PADDLE_LEFT_BLUE     0 
#define PADDLE_RIGHT_RED     0 
#define PADDLE_RIGHT_GREEN 100 
#define PADDLE_RIGHT_BLUE    0 
#define BALL_VELOCITY       10


//// Debugging
#define DEBUG            1     // Choose whether to display certain messages 


