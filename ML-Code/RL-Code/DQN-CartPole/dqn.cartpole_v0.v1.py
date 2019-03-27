#!/usr/bin/env python
# coding: utf-8

# In[2]:


import sys, os
import numpy as np
# import matplotlib.pyplot as plt

import tensorflow as tf
import tensorflow.keras.layers as layers
import tensorflow.keras.losses as losses
import tensorflow.keras.optimizers as optim

tf.__version__, tf.executing_eagerly()


# In[9]:


from collections import deque
import random 

class DQN_Model:
    def __init__(self, num_actions, num_features, learning_rate=0.02,
                        reward_decay=0.95, e_greedy=0.95, replace_target_iter=500,
                        memory_size=5000, batch_size=32, e_greedy_increment=None,
                        output_graph=False, memory_neg_p = 0.5):
#         super(DQN_Model, self).__init__()
        # ____define_some_parameters____
        self.num_actions = num_actions
        self.num_features = num_features
        self.lr = learning_rate
        self.gamma = reward_decay
        self.epsilon_max = e_greedy
        self.replace_target_iter = replace_target_iter # 冻结参数的间隔
        self.memory_size = memory_size
        self.batch_size = batch_size
        self.epsilon_increment = e_greedy_increment
        self.epsilon = 0 if e_greedy_increment is not None else self.epsilon_max
        self.learn_step_counter = 0
        
        self.memory_neg = deque(maxlen = int(self.memory_size*memory_neg_p))
        self.memory_pos = deque(maxlen = self.memory_size - int(self.memory_size*memory_neg_p))
        self.memory_neg_p = memory_neg_p

        # ____Call_fn_build_network____
        self.__build_network()
        

    def __define_network(self, name):
        inputs = layers.Input(shape=(self.num_features,))
        x = layers.Dense(32, activation='sigmoid')(inputs)
        x = layers.Dense(32, activation='sigmoid')(x)
        return inputs, layers.Dense(self.num_actions, name=name+'_output')(x)
        
    def __build_network(self):
        # 构建eval/target网络，注意这个target层输出是q_next而不是，算法中的q_target
        eval_inputs, self.q_eval = self.__define_network('Eval')          # 具有最新的参数
        target_inputs, self.q_next = self.__define_network('Target')   # 被冻结的参数
        self.targetNet = tf.keras.Model(target_inputs, self.q_next)
        self.evalNet = tf.keras.Model(eval_inputs, self.q_eval, name = 'DQN_Eval_Net')
        
        rmsprop = optim.RMSprop(lr=self.lr)
        self.targetNet.compile(loss='mean_squared_error', optimizer=rmsprop, metrics=['accuracy'])
        self.evalNet.compile(loss='mean_squared_error', optimizer=rmsprop, metrics=['accuracy'])
        
    def do_target_replacement(self):
        self.targetNet.set_weights(self.evalNet.get_weights())
        print("!! Params has changed!")
    
    def store_transition(self, s, a, r, s_, terminal):
        if terminal:
            self.memory_neg.append((s, a, r, s_, terminal))
        else:
            self.memory_pos.append((s, a, r, s_, terminal))
    def replay_transition(self):
        batch_mem_pos = random.sample(self.memory_pos, self.batch_size-int(self.batch_size*self.memory_neg_p))
        batch_mem_neg = random.sample(self.memory_neg, int(self.batch_size*self.memory_neg_p))
        batch_mem = batch_mem_pos+batch_mem_neg
        s  = np.array([d[0] for d in batch_mem])
        a  = np.array([d[1] for d in batch_mem], dtype=np.int32)
        r  = np.array([d[2] for d in batch_mem])
        s_ = np.array([d[3] for d in batch_mem])
        t  = np.array([d[4] for d in batch_mem])
        return s, a, r, s_, t

    def choose_action(self, obs):
        if np.random.uniform() < self.epsilon:
            action_dist = self.evalNet.predict(obs)
            action = np.argmax(action_dist)
        else:
            action = np.random.randint(0, self.num_actions)
        return action
        
    def learn(self):
        # ____Replace_Target_weights____
        if self.learn_step_counter % self.replace_target_iter == 0:
            self.do_target_replacement()
            
        
        # ____Sample_Memory____
        if len(self.memory_neg)+len(self.memory_pos) > self.batch_size:
            s, eval_act, reward, s_, t = self.replay_transition()

        q_next, q_eval = self.targetNet.predict(s_), self.evalNet.predict(s)
        
        q_target = q_eval.copy()
        
        batch_index = np.arange(self.batch_size, dtype=np.int32)
#         print(eval_act)
        q_target[batch_index, eval_act] = reward + self.gamma * np.max(q_next, axis=1)
        q_target[t, eval_act[t]] = reward[t]
#         print(q_target)
        if self.learn_step_counter % self.replace_target_iter == 0:
            print(np.append(q_eval,q_target, axis=1))

        self.evalNet.fit(s, q_target, epochs=10, verbose=False)
        
        self.epsilon = self.epsilon + self.epsilon_increment if self.epsilon < self.epsilon_max else self.epsilon_max
        self.learn_step_counter += 1
    
    def summary(self):
        self.targetNet.summary()
        return self.evalNet.summary()
    


# In[10]:


# q_target = np.array([[1,2],[1,2],[1,2]])
# batch_index = np.array([0,1,2])
# eval_act = np.array([0,1,0])
# q_target[batch_index, eval_act]


# In[11]:


import gym


# In[12]:


env = gym.make('CartPole-v0')
model = DQN_Model(num_actions=2, num_features=4, e_greedy=0.99, e_greedy_increment=0.002)
model.summary()
obs = env.reset()


# In[13]:


# step = 0
# for times in range(10000):
#     obs = env.reset()
#     total_reward  = 0
#     while True:
#         env.render()
#         action = model.choose_action(obs[None, :])
        
#         obs_, reward, terminal, _ = env.step(action)
#         total_reward += reward

#         if total_reward < 200:
#             model.store_transition(obs, action, reward, obs_, terminal)
#         else:
#             print('Good Job!')
        
        
        
#         if (step > 1000) and (step % 3 == 0):
#             model.learn()
        
#         obs = obs_
        
#         if terminal:
#             env.render()
#             # print(reward, terminal)
#             break
#         step += 1
        
#     print('Time: ', times, 'Gain_reward: ', total_reward, 'Steps: ', step)
#     if times%200 == 199:
#         model.evalNet.save_weights('./dqn.new.weights')

print(model.evalNet.load_weights('./dqn.ori.weights'))
model.epsilon = 0.99
step = 0
for times in range(10000):
    obs = env.reset()
    total_reward  = 0
    while True:
        env.render()
        action = model.choose_action(obs[None, :])
        
        obs_, reward, terminal, _ = env.step(action)

        total_reward += reward

        if terminal:
            env.render()
            break
        step += 1
        
    print('Time: ', times, 'Gain_reward: ', total_reward, 'Steps: ', step)
