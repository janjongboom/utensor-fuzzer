import tensorflow as tf
import numpy as np
import sys
import json
import math

def load_graph(frozen_graph_filename):
    # We load the protobuf file from the disk and parse it to retrieve the
    # unserialized graph_def
    with tf.gfile.GFile(frozen_graph_filename, "rb") as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())

    # Then, we can use again a convenient built-in function to import a graph_def into the
    # current default Graph
    with tf.Graph().as_default() as graph:
        tf.import_graph_def(
            graph_def,
            input_map=None,
            return_elements=None,
            name="prefix",
            op_dict=None,
            producer_op_list=None
        )
    return graph

# We use our "load_graph" function
graph = load_graph("./model/trained.pb")

# We access the input and output nodes
x = graph.get_tensor_by_name('prefix/x_input:0')
y = graph.get_tensor_by_name('prefix/y_pred/Softmax:0')

# We launch a Session
with tf.Session(graph=graph) as sess:
    input = np.loadtxt(sys.argv[1])

    # compute the predicted output for test_x
    pred_y = sess.run( y, feed_dict={x: input} )

    print('Begin output')
    print(json.dumps(pred_y.tolist(), separators=(',', ':')))
    print('End output')
