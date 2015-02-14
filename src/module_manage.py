from cnn import *

class Manage(object):

    def __init__(self, dataset_url = "/home/liugt/test.pkl"):
        self.dataset_url = dataset_url
        self.batch_size = 8
        self.n_out = 16
        self.learning_rate = 0.0001  
    def engine_start(self):
        face_cnn = cnn()
        self.face_cnn = face_cnn
        face_cnn.simple_deepid(learning_rate=self.learning_rate, n_epochs=2, dataset=self.dataset_url, nkerns=[20,40,60,80], batch_size=self.batch_size, n_hidden=160, n_out=self.n_out, acti_func=relu)
        
    def getPrediction(self, path):
        self.face_cnn.setPredictionData(path)
        predict_result = self.face_cnn.prediction()
        tem = '%d' %predict_result[0]
        return tem

if __name__ == '__main__':
    manage = Manage()
    manage.engine_start()
    
