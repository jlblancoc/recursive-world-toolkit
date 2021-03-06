/* +---------------------------------------------------------------------------+
   |                       Recursive World Toolkit                             |
   |                                                                           |
   |   Copyright (C) 2011-2015  Jose Luis Blanco Claraco                       |
   |                                                                           |
   |      RWT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |    RWT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with  RWT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+
 */

#include "rwt.h"

#include <mrpt/opengl.h>

using namespace rwt;
using namespace std;

/** Build an OpenGL representation of the world */
void rwt::world_to_opengl(const RWT_World &the_world,
                          mrpt::opengl::CSetOfObjects &out_gl,
                          const WRL_RenderOptions &opts) {
  out_gl.clear();

  if (opts.show_world_origin_corner)
    out_gl.insert(mrpt::opengl::stock_objects::CornerXYZSimple(
        opts.world_origin_corner_params.scale,
        opts.world_origin_corner_params.line_width));

  mrpt::opengl::CPointCloud::Ptr gl_LMs = mrpt::opengl::CPointCloud::Create();
  gl_LMs->loadFromPointsMap(&the_world.landmarks);
  gl_LMs->setPointSize(2.0);
  gl_LMs->setColor_u8(220, 220, 220);
  gl_LMs->setName("landmarks");

  mrpt::opengl::CPointCloud::Ptr gl_Nodes = mrpt::opengl::CPointCloud::Create();
  gl_Nodes->loadFromPointsMap(&the_world.nodes);
  gl_Nodes->setPointSize(5.0);
  gl_Nodes->setColor_u8(255, 0, 0);

  // Add node labels in a single object, so it can be shown/hiden by name:
  mrpt::opengl::CSetOfObjects::Ptr gl_NodeLabels =
      mrpt::opengl::CSetOfObjects::Create();
  for (size_t i = 0; i < the_world.nodes.size(); i++) {
    float x, y, z;
    the_world.nodes.getPoint(i, x, y, z);

    mrpt::opengl::CText::Ptr gl_txt = mrpt::opengl::CText::Create();
    gl_txt->setLocation(x, y, z);
    gl_txt->setString(mrpt::format("%u", static_cast<unsigned int>(i)));

    gl_NodeLabels->insert(gl_txt);
  }
  gl_NodeLabels->setName("node_labels");
  gl_NodeLabels->setVisibility(false);

  mrpt::opengl::CSetOfLines::Ptr gl_edges = mrpt::opengl::CSetOfLines::Create();
  gl_edges->setLineWidth(1);
  gl_edges->setColor_u8(TColor(0, 0, 220));

  for (RWT_adjacency_graph::const_iterator it = the_world.graph.begin();
       it != the_world.graph.end(); ++it) {
    const size_t idx1 = it->first.first;
    float x1, y1, z1;
    the_world.nodes.getPoint(idx1, x1, y1, z1);

    const size_t idx2 = it->first.second;
    float x2, y2, z2;
    the_world.nodes.getPoint(idx2, x2, y2, z2);

    gl_edges->appendLine(x1, y1, z1, x2, y2, z2);
  }

  out_gl.insert(gl_LMs);
  out_gl.insert(gl_Nodes);
  out_gl.insert(gl_NodeLabels);
  out_gl.insert(gl_edges);
}
