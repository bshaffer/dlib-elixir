# libdlib.ex
defmodule Dlib do
  @on_load :load_nifs

  def load_nifs do
    :erlang.load_nif('./frontal_face_detector', 0)
  end

  def frontal_face_detector(_a) do
    raise "NIF frontal_face_detector/1 not implemented"
  end
end
