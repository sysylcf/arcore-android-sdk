/*
 * Copyright 2017 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef C_ARCORE_CLOUD_ANCHOR_UTIL_H_
#define C_ARCORE_CLOUD_ANCHOR_UTIL_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/log.h>
#include <errno.h>
#include <jni.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "arcore_c_api.h"
#include "glm.h"

#ifndef LOGI
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, "cloud_anchor_example_c", __VA_ARGS__)
#endif  // LOGI

#ifndef LOGE
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "cloud_anchor_example_c", __VA_ARGS__)
#endif  // LOGE

#ifndef CHECK
#define CHECK(condition)                                                   \
  if (!(condition)) {                                                      \
    LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    abort();                                                               \
  }
#endif  // CHECK

namespace cloud_anchor {

// Utilities for C hello AR project.
namespace util {

// Provides a scoped allocated instance of Anchor.
// Can be treated as an ArAnchor*.
class ScopedArPose {
 public:
  explicit ScopedArPose(const ArSession* session) {
    ArPose_create(session, nullptr, &pose_);
  }
  ~ScopedArPose() { ArPose_destroy(pose_); }
  ArPose* GetArPose() { return pose_; }
  // Delete copy constructors.
  ScopedArPose(const ScopedArPose&) = delete;
  void operator=(const ScopedArPose&) = delete;

 private:
  ArPose* pose_;
};

// Check GL error, and abort if an error is encountered.
//
// @param operation, the name of the GL function call.
void CheckGlError(const char* operation);

// Create a shader program ID.
//
// @param vertex_source, the vertex shader source.
// @param fragment_source, the fragment shader source.
// @return
GLuint CreateProgram(AAssetManager* mgr, const char* vertex_shader_file_name,
                     const char* fragment_shader_file_name);

// Load a text file from assets folder.
//
// @param mgr, AAssetManager pointer.
// @param file_name, path to the file, relative to the assets folder.
// @param out_string, output string.
// @return true if the file is loaded correctly, otherwise false.
bool LoadTextFileFromAssetManager(AAssetManager* mgr, const char* file_name,
                                  std::string* out_file_text_string);

// Load png file from assets folder and then assign it to the OpenGL target.
// This method must be called from the renderer thread since it will result in
// OpenGL calls to assign the image to the texture target.
//
// @param target, openGL texture target to load the image into.
// @param path, path to the file, relative to the assets folder.
// @return true if png is loaded correctly, otherwise false.
bool LoadPngFromAssetManager(int target, const std::string& path);

// Load obj file from assets folder from the app.
//
// @param mgr, AAssetManager pointer.
// @param file_name, name of the obj file.
// @param out_vertices, output vertices.
// @param out_normals, output normals.
// @param out_uv, output texture UV coordinates.
// @param out_indices, output triangle indices.
// @return true if obj is loaded correctly, otherwise false.
bool LoadObjFile(AAssetManager* mgr, const std::string& file_name,
                 std::vector<GLfloat>* out_vertices,
                 std::vector<GLfloat>* out_normals,
                 std::vector<GLfloat>* out_uv,
                 std::vector<GLushort>* out_indices);

// Show the resolve room code dialog.
void ShowResolveDialog();

// Display a message on the lower snackbar.
void DisplayMessageOnLowerSnackbar(const std::string& message);

// Updates the firebase room code label.  If getNewRoomCode is true, it will
// update the roomcode to a value dynamically fetched from Firebase.  Otherwise,
// the value in optional_new_room_code will be used.
void UpdateFirebaseRoomCode(bool getNewRoomCode,
                            std::int64_t optional_new_room_code);

// Updates firebase with a hosted anchor, if possible.
void MaybeUpdateFirebase(const std::string& message);

enum class HostResolveVisibilityEnum { ALL, ONLY_HOST, ONLY_RESOLVE };

void SetHostAndResolveButtonVisibility(HostResolveVisibilityEnum visiblity);

// Format and output the matrix to logcat file.
// Note that this function output matrix in row major.
void Log4x4Matrix(float raw_matrix[16]);

// Get transformation matrix from ArAnchor.
void GetTransformMatrixFromAnchor(ArSession* ar_session,
                                  const ArAnchor* ar_anchor,
                                  glm::mat4* out_model_mat);

// Calculate the normal distance to plane from cameraPose, the given planePose
// should have y axis parallel to plane's normal, for example plane's center
// pose or hit test pose.
float CalculateDistanceToPlane(ArSession* ar_session, ArPose* plane_pose,
                               const ArPose& camera_pose);
}  // namespace util
}  // namespace cloud_anchor

#endif  // C_ARCORE_CLOUD_ANCHOR_UTIL_H_
