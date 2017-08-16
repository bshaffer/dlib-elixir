#include "erl_nif.h"

#include "dlib/matrix.h"
#include "dlib/pixel.h"
#include "dlib/image_processing/frontal_face_detector.h"

int get_pixel_row(ErlNifEnv* env, ERL_NIF_TERM term, dlib::rgb_pixel pixel_arr[] );
dlib::rgb_pixel get_pixel(ErlNifEnv* env, ERL_NIF_TERM term);

static ERL_NIF_TERM
frontal_face_detector(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  ERL_NIF_TERM row_head, row_tail, row_tail_tmp;
  row_tail = argv[0];
  row_tail_tmp = row_tail;

  unsigned int rows, columns;
  enif_get_list_length(env, row_tail, &rows);

  // get the first row in order to count the columns
  enif_get_list_cell(env, row_tail, &row_head, &row_tail_tmp);
  enif_get_list_length(env, row_head, &columns);

  dlib::matrix<dlib::rgb_pixel> m;
  m.set_size(rows, columns);

  int i = 0;
  while(enif_get_list_cell(env, row_tail, &row_head, &row_tail))
  {
      ERL_NIF_TERM col_head, col_tail;
      col_tail = row_head;

      int j = 0;
      while(enif_get_list_cell(env, col_tail, &col_head, &col_tail))
      {
          unsigned int length;
          enif_get_list_length(env, col_head, &length);
          if (length != 3) {
            return enif_make_badarg(env);
          }
          m(i, j++) = get_pixel(env, col_head);
      }
      i++;
  }

  dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

  std::vector<dlib::rectangle> result = detector(m);

  ERL_NIF_TERM rect_array[result.size()];

  int k = 0;
  for (auto & rect : result) {
      ERL_NIF_TERM ex_rect = enif_make_list4(
        env,
        enif_make_int(env, rect.left()),
        enif_make_int(env, rect.top()),
        enif_make_int(env, rect.right()),
        enif_make_int(env, rect.bottom())
      );
      rect_array[k++] = ex_rect;
  }

  return enif_make_list_from_array(env, rect_array, result.size());
}

//helper method to process a pixel
dlib::rgb_pixel get_pixel(ErlNifEnv* env, ERL_NIF_TERM term)
{
    int int_arr[3];
    ERL_NIF_TERM head, tail;
    tail = term;

    int i = 0;
    while(enif_get_list_cell(env, tail, &head, &tail))
    {
        int request_int;
        enif_get_int(env, head, &request_int);
        int_arr[i++] = request_int;
    }
    return dlib::rgb_pixel(int_arr[0], int_arr[1], int_arr[2]);
}


// Let's define the array of ErlNifFunc beforehand:
static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
  {"frontal_face_detector", 1, frontal_face_detector}
};

ERL_NIF_INIT(Elixir.Dlib, nif_funcs, NULL, NULL, NULL, NULL)
