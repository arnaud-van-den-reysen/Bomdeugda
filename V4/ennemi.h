typedef struct coord COORD;
typedef struct node NODE;

int** convert_to_matrix(char* adress);
int*** make_dist_matrix(int** matrix, int rows, int cols);
void print_3d_int_mat(int*** matrix, int n, int m);
void print_int_mat(int** matrix);
void print_coord(COORD pos);
void print_list(NODE* list);
NODE* new_head(NODE* list, COORD pos);
NODE* delete_pos(NODE* list, int i, int j);

COORD get_dustmen(int** matrix, int rows, int cols);
COORD get_ennemi(int** matrix, int rows, int cols);
NODE* get_dumpsters(int** matrix, int rows, int cols);
NODE* visit(int*** dist_map, NODE* list, int rows, int cols, int i_prev, int j_prev, int distance, int i, int j);
int*** Dijkstra(int** matrix, int rows, int cols, int i0, int j0);
NODE* follow_trail(int*** dist_map, int i0, int j0, COORD nearest);
NODE* get_trajectory(int** matrix, int rows, int cols,COORD joueur, NODE* ennemi);
bool avance_ennemi(bool av,int posX,int posY);
