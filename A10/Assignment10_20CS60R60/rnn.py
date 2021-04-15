
#CS60012 : Computing lab II
#Spring 2021
#Assignment - 10 -Task1
#Cloth Classification using RNN and CNN
#Submitted by : Ravi pratap singh (20CS60R60)

# This code is also present in google colab :
# https://colab.research.google.com/drive/1LKQET7Ql_T0Pfukz8j1aVWFkOOCfodMA?usp=sharing

import warnings
warnings.simplefilter(action='ignore', category=DeprecationWarning)
# TensorFlow
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()
# Helper libraries
import numpy as np
import os
import sys
import matplotlib.pyplot as plt
import platform
from sklearn.model_selection import train_test_split

# argparse used to take command line arguments
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--train", help="train the model",action="store_true")
parser.add_argument("--test", help="test the model",action="store_true")
args = parser.parse_args()

def reset_graph(seed=101): ### it resests all created graph, it's required once re-defining of any placeholders, variables, shapes or model structures is needed
    tf.reset_default_graph()
    tf.set_random_seed(seed)
    np.random.seed(seed)

def validation_batch(x_train, y_train, batch_size): ### it takes random permutation of lenght x_train and splits x_train (together with y_train) into batches number
    random_idx  = np.random.permutation(len(x_train))
    n_batches   = len(x_train) // batch_size
    for batch_idx in np.array_split(random_idx, n_batches):
        x_batch, y_batch = x_train[batch_idx], y_train[batch_idx]
        yield x_batch, y_batch

def train(x_train,y_train,best_learning_rate,draw_plot):
    
    n_neurons   = 100             ### number of neurons in the network i.e hidden state size
    n_outputs   = 10              ### outputs that represent digits from 0-9
    n_inputs    = 28              ### lengh of each row ,Since the pictures are of the shape  28𝑥28 pixels
    time_steps  = 28              ### Timesteps for unfolding the RNN
    n_epochs    = 400             ### Total No. of epochs
    batch_size  = 200             ### Traning validation size
    retention_prob  = 0.8         ### dropout with retention probability of 0.8
    n_layers        = 3           ### number of BasicRNNCell layers
    patience = 30                 ### of max_epochs_without_progress, the model stops and saves last parameters

    #In case once placeholders are run twice and doubled in the graph
    reset_graph()

    print("+" * 80)
    print("Number of Neurons             --- ",n_neurons)
    print("Number of hidden layers       --- ",n_layers)
    print("Learning rate                 --- ",best_learning_rate)
    print("Retention probability         --- ",retention_prob)
    print("Batch size                    --- ",batch_size)
    print("Number of epochs              --- ",n_epochs)
    print("Patience                      --- ",patience)
    print("+" * 80)

    #Together with two main placeholders created here the additional one. 
    #It is needed for the DropOut layer.
    with tf.name_scope("Inputs"):
        X           = tf.placeholder(tf.float32, [None, time_steps, n_inputs], name="X")
        y           = tf.placeholder(tf.int32, [None], name="y")
        keep_prob   = tf.placeholder_with_default(1.0, shape=(), name='keep_probability')
    
    print("Inputs --- >")
    print(X)
    print(y)
    #print(keep_prob)

    # Sequncing the input for static rnn
    X_seq = tf.unstack(X, axis=1) # shape: [batch_size, i, n_inputs]

    #By default keep_prob is equal to 1.0 and it will be changed during training only 
    # (to avoid dropping during evaluation on validation set)

    with tf.name_scope("Basic_RNN_Layers"):
        lstm_cells        = [tf.nn.rnn_cell.BasicRNNCell(num_units = n_neurons, activation = tf.nn.tanh) for layer in range(n_layers)]
        lstm_cells_drop   = [tf.nn.rnn_cell.DropoutWrapper(cell, input_keep_prob=keep_prob) for cell in lstm_cells]
        multi_layer_cell  = tf.nn.rnn_cell.MultiRNNCell(lstm_cells_drop)
        outputs, states   = tf.nn.static_rnn(multi_layer_cell, X_seq, dtype = tf.float32) ### states return final state (last output) of the multi_layer_cell
    
    #In the Loss scope there are three dense layers with 64, 32 and 10 outputs, 
    #Then the sparse_soft_max_cross_entropy_with_logits

    with tf.name_scope("Loss"):
        states_concat   = tf.concat(axis=1, values=states, name='states_reshape')
        dense_layer1    = tf.layers.dense(states_concat, 64, name='dense_1')
        dense_layer2    = tf.layers.dense(dense_layer1, 32, name='dense_2')
        logits          = tf.layers.dense(dense_layer2, n_outputs, name='output_layer')
        cross_entropy   = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=y, logits=tf.reshape(logits, shape=(-1, n_outputs)), name='softmax_cross_entropy')
        loss            = tf.reduce_mean(cross_entropy, name='loss')
        loss_summary    = tf.summary.scalar('loss_summary', loss)
    
    #Defining training variables like learning rates
    with tf.name_scope("Train"):    
        optimizer          = tf.train.AdamOptimizer(learning_rate=best_learning_rate,beta1=0.9, beta2=0.999, epsilon=1e-08, name='Adam_optimizer')
        training_optimizer = optimizer.minimize(loss, name='training_Adam')  ###minimize cross entropy
    
    #Defining the accuracy_summary variable.
    with tf.name_scope("Evaluation"):        
        correct          = tf.nn.in_top_k(tf.reshape(logits, (-1, n_outputs)), y, 1, name='inTopK')
        accuracy         = tf.reduce_mean(tf.cast(correct, tf.float32), name='Accuracy')
        accuracy_summary = tf.summary.scalar('Accuracy_Summary', accuracy)

    #initializing session
    init    = tf.global_variables_initializer()
    saver   = tf.train.Saver()

    #paths for saving the session
    checkpoint_path     = "./checkpoints/mnist_rnn_model.ckpt"
    final_model_path    = "./log_rnn/log_rnn"

    best_loss               = np.infty      ### parameters for early stopping
    epochs_without_progress = 0             ### once epochs_without_progress reaches the value
    train_acc_list, val_acc_list, train_loss_list, val_loss_list = [], [], [], [] 

    # Evaluation and analysis.
    #Splitting training set into validation
    x_train, x_valid, y_train, y_valid = train_test_split(x_train, y_train,test_size=0.1, random_state=101)
    x_valid = x_valid.reshape((-1, time_steps, n_inputs)) ### reshaping val set
    
    print("+" * 80)
    print("test - val split into 80:20")
    print("Length of Training Data:", len(x_train))
    print("Length of Validation Data", len(x_valid))
    print("+" * 80)
    
    with tf.Session() as sess:
        init.run() 
        for epoch in range(n_epochs):
            for x_batch, y_batch in validation_batch(x_train, y_train, batch_size): ### generating batches for x/y_train
                x_batch = x_batch.reshape((-1, time_steps, n_inputs)) ### reshape to the format define for X placeholder
            
                ### for x_batch/y_batch data feed keep_prob(retention prob) values for 0.8
                sess.run(training_optimizer, feed_dict={X: x_batch, y: y_batch, keep_prob: retention_prob})
            acc_batch, loss_batch, acc_sum, loss_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_batch, y: y_batch, keep_prob: retention_prob})   
        
            ### during evaluation on val set, no dropout required, keep_prob has default value (1.0)    
            acc_val, loss_val, acc_val_sum, loss_val_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_valid, y: y_valid})
               
            train_acc_list.append(round(float(acc_batch * 100), 3))        #### if not using tensorboard,
            train_loss_list.append(loss_batch)      #### temporary parameters of train/val accuracy and loss 
            val_acc_list.append(round(float(acc_val * 100), 3))        #### can be placed in lists and plotted accordingly
            val_loss_list.append(loss_val)  

            #Considering every three epoch only
            if epoch % 5 == 0:
                print("Epoch", epoch,'\tTrain accuracy: {:.3f}%'.format(acc_batch * 100), '\tValidation accuracy: {:.3f}%'.format(acc_val * 100), '\tLoss: {:.3f}'.format(loss_batch))
                saver.save(sess, checkpoint_path)
                if loss_batch < best_loss:
                    saver.save(sess, final_model_path)
                    best_loss = loss_batch
                else:
                    epochs_without_progress += 2
                    if epochs_without_progress > patience:
                        print('Early Stopping')
                        break
    
    if draw_plot == True:
        # plot creation of Train Accuracy vs validation Accuracy
        fig, (axis1, axis2) = plt.subplots(1, 2, figsize=(20,6))
        fig.suptitle('Evaluation results')
        axis1.set_title("Train Accuracy vs validation Accuracy")
        axis1.plot(range(len(val_acc_list)), train_acc_list ,label="Train Accuracy")
        axis1.plot(range(len(val_acc_list)), val_acc_list, label="val Accuracy")
        axis1.legend(loc='lower right')

        # plot creation of Train Accuracy vs val Accuracy
        axis2.set_title("Train Loss vs val Loss")
        axis2.plot(range(len(val_acc_list)), train_loss_list,label="Train Loss")
        axis2.plot(range(len(val_acc_list)), val_loss_list, label="val Loss")
        axis2.legend(loc='best')

    #return val_acc_list

def test(x_test,y_test,plot_predictions):
    #parameters for reshaping
    size    = -1
    nrows   = 28
    ncols   = 28
    with tf.Session() as sess:
        ## Add ops to save and restore all the variables.
        saver = tf.train.import_meta_graph('./log_rnn/log_rnn.meta')
        saver.restore(sess, tf.train.latest_checkpoint('./log_rnn/'))
        current_graph = tf.compat.v1.get_default_graph()

        # Launch the model, initialize the variables, do predictions
        # Accessing different variables by the name_scope they have been saved
        loss        = current_graph.get_tensor_by_name("Loss/loss:0")
        accuracy    = current_graph.get_tensor_by_name("Evaluation/Accuracy:0")             
        X           = current_graph.get_tensor_by_name("Inputs/X:0")
        y           = current_graph.get_tensor_by_name("Inputs/y:0") 
        logits      = current_graph.get_tensor_by_name("Loss/output_layer/MatMul:0")
        x_test      = x_test.reshape((size, nrows, ncols)) ### reshaping test set

        #Run the operation by feeding input
        loss_test, acc_test = sess.run([loss, accuracy], feed_dict={X: x_test, y: y_test})
        perm                = np.random.permutation(len(x_test))
        
        #calculating accuracy
        count = 0
        probabilities = logits.eval(feed_dict={X: x_test, y: y_test})
        for i in range(len(perm)):
            if np.argmax(probabilities[perm[i]]) == y_test[perm[i]]:
                count+=1
        print('Test Loss: {:.3f} \t Test Accuracy: {:.3f}'.format(loss_test , count/100))
        
        if plot_predictions:
            Length=1
            Width=8
            axis = []
            for i in range(8):
                ax_s.append(str("ax")+ str(i))
            fig, (axis) = plt.subplots(Length,Width, figsize=(22,6))
            perm = perm[:8]
            for i in range(8):
                title = "Predicted value:" + str(np.argmax(probabilities[perm[i]])) + ",\nTrue value: " + str(y_test[perm[i]])
                axis[i].set_title(title)
                axis[i].imshow(x_test[perm[i]],cmap =plt.cm.gray_r, interpolation = "nearest") 

def main():
    print("TensorFlow Version\nRunning: ",tf.__version__)
    print('Eager execution enabled: ', tf.executing_eagerly()) 
    print("Python Version\nRunning: ", platform.python_version())

    (x_train, y_train), (x_test, y_test) = tf.keras.datasets.fashion_mnist.load_data() ### loading the datasets
    x_train     = x_train.astype(np.float32).reshape(-1, 28*28) / 255.0 ### reshaping and normalizing
    x_test      = x_test.astype(np.float32).reshape(-1, 28*28) / 255.0   ### reshaping and normalizing
    y_train     = y_train.astype(np.int32)
    y_test      = y_test.astype(np.int32)

    print("Length of the training set:", len(x_train))
    print("Length of the test set:", len(x_test))

    class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat', 
                    'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']
    
    total_class = len(class_names)
    if args.train:
        # learning_rates=[0.05,0.005,0.001,0.0005,0.0001]
        # learning_accuracy=[]
        # epoch_length=[]
        # for rate in learning_rates:
        #     accuracy=train(x_train,y_train,rate)
        #     learning_accuracy.append(accuracy[-1])
        print("Training Phase")
        print("+" * 80)
        train(x_train,y_train,0.0005,False)
        print("+" * 80)
        print("Finished Training")
    if args.test:
        print("Testing Phase")
        print("+" * 80)
        test(x_test,y_test,False)
        print("+" * 80)
        print("Finished Testing")
    
if __name__ == "__main__":
	main()