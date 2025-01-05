#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <map>

using namespace std;

// Helper functions for dataset processing
vector<vector<double>> loadDataset(const string &filename, vector<int> &labels, int mode) {
    ifstream file(filename);
    string line;
    vector<vector<double>> data;

    // For probabilistic replacement
    vector<map<string, double>> probabilities;

    // First pass to calculate probabilities
    if (mode == 1) {
        vector<map<string, int>> counts;
        int num_columns = -1;

        while (getline(file, line)) {
            istringstream stream(line);
            string value;
            int index = 0;

            while (getline(stream, value, ',')) {
                if (index == 0) {
                    // Skip label
                } else {
                    if (counts.size() <= index - 1) {
                        counts.emplace_back(map<string, int>());
                    }
                    counts[index - 1][value]++;
                }
                index++;
            }
            if (num_columns == -1) {
                num_columns = index;
            }
        }

        for (const auto &count_map : counts) {
            double total = 0;
            for (const auto &pair : count_map) {
                total += pair.second;
            }
            map<string, double> prob_map;
            for (const auto &pair : count_map) {
                prob_map[pair.first] = pair.second / total;
            }
            probabilities.push_back(prob_map);
        }
        file.clear();
        file.seekg(0, ios::beg);
    }

    // Second pass to process the data
    while (getline(file, line)) {
        istringstream stream(line);
        string value;
        vector<double> row;
        int label;
        int index = 0;

        while (getline(stream, value, ',')) {
            if (index == 0) {
                // Process label
                label = (value == "republican") ? 1 : 0;
            } else {
                // Process feature
                if (value == "n")
                    row.push_back(0);
                else if (value == "y")
                    row.push_back(1);
                else if (mode == 1) {
                    // Replace "?" with probabilistic distribution
                    double prob = (double)rand() / RAND_MAX;
                    if (prob <= probabilities[index - 1]["y"])
                        row.push_back(1);
                    else
                        row.push_back(0);
                } else
                    row.push_back(2); // '?' or missing as a third value
            }
            index++;
        }

        labels.push_back(label);
        data.push_back(row);
    }
    file.close();
    return data;
}

// Naive Bayes classifier
class GaussianNaiveBayes {
    vector<double> class_prior;
    vector<vector<double>> mean;
    vector<vector<double>> variance;
    double var_smoothing;

public:
    GaussianNaiveBayes(double smoothing) : var_smoothing(smoothing) {}

    void fit(const vector<vector<double>> &X, const vector<int> &y) {
        size_t n_features = X[0].size();
        size_t n_classes = 2; // Binary classification

        class_prior = {0, 0};
        mean = vector<vector<double>>(n_classes, vector<double>(n_features, 0));
        variance = vector<vector<double>>(n_classes, vector<double>(n_features, 0));

        vector<int> class_count(n_classes, 0);

        for (size_t i = 0; i < X.size(); ++i) {
            int cls = y[i];
            class_prior[cls]++;
            class_count[cls]++;
            for (size_t j = 0; j < n_features; ++j) {
                mean[cls][j] += X[i][j];
            }
        }

        for (int cls = 0; cls < n_classes; ++cls) {
            class_prior[cls] /= X.size();
            for (size_t j = 0; j < n_features; ++j) {
                mean[cls][j] /= class_count[cls];
            }
        }

        for (size_t i = 0; i < X.size(); ++i) {
            int cls = y[i];
            for (size_t j = 0; j < n_features; ++j) {
                double diff = X[i][j] - mean[cls][j];
                variance[cls][j] += diff * diff;
            }
        }

        for (int cls = 0; cls < n_classes; ++cls) {
            for (size_t j = 0; j < n_features; ++j) {
                variance[cls][j] = (variance[cls][j] + var_smoothing) / (class_count[cls] + var_smoothing);
            }
        }
    }

    vector<int> predict(const vector<vector<double>> &X) {
        vector<int> predictions;

        for (const auto &x : X) {
            vector<double> log_probs(2, 0);

            for (int cls = 0; cls < 2; ++cls) {
                log_probs[cls] = log(class_prior[cls]);

                for (size_t j = 0; j < x.size(); ++j) {
                    double diff = x[j] - mean[cls][j];
                    log_probs[cls] += -0.5 * log(2 * M_PI * variance[cls][j]) - (diff * diff) / (2 * variance[cls][j]);
                }
            }

            predictions.push_back((log_probs[0] > log_probs[1]) ? 0 : 1);
        }

        return predictions;
    }
};

// Evaluation metrics
double accuracy_score(const vector<int> &y_true, const vector<int> &y_pred) {
    int correct = 0;
    for (size_t i = 0; i < y_true.size(); ++i) {
        if (y_true[i] == y_pred[i]) {
            correct++;
        }
    }
    return static_cast<double>(correct) / y_true.size();
}

int main() {
    string filename = "house-votes-84.data";

    int mode;
    cout << "Enter mode (0 for third option '?' or 1 for probabilistic distribution): ";
    cin >> mode;

    vector<int> labels;
    vector<vector<double>> data = loadDataset(filename, labels, mode);

    // Split data into training and testing sets
    int test_size = data.size() * 0.2;
    vector<vector<double>> X_train(data.begin(), data.end() - test_size);
    vector<int> y_train(labels.begin(), labels.end() - test_size);
    vector<vector<double>> X_test(data.end() - test_size, data.end());
    vector<int> y_test(labels.end() - test_size, labels.end());

    // Choose lambda values
    vector<double> lambda_values = {0.1, 0.5, 1.0, 2.0, 5.0};
    double best_lambda = 0;
    double best_accuracy = 0;

    for (double lambda_val : lambda_values) {
        GaussianNaiveBayes model(lambda_val);
        model.fit(X_train, y_train);

        vector<int> y_pred = model.predict(X_test);
        double accuracy = accuracy_score(y_test, y_pred);
        cout << "Accuracy with lambda=" << lambda_val << ": " << accuracy * 100 << "%" << endl;

        if (accuracy > best_accuracy) {
            best_accuracy = accuracy;
            best_lambda = lambda_val;
        }
    }

    cout << "\nBest lambda: " << best_lambda << " with accuracy: " << best_accuracy * 100 << "%" << endl;

    // 10-fold cross-validation
    random_device rd;
    mt19937 g(rd());
    vector<int> indices(X_train.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), g);

    int k = 10;
    int fold_size = X_train.size() / k;
    vector<double> fold_accuracies;

    for (int i = 0; i < k; ++i) {
        vector<int> test_indices(indices.begin() + i * fold_size, indices.begin() + (i + 1) * fold_size);
        vector<int> train_indices;
        train_indices.insert(train_indices.end(), indices.begin(), indices.begin() + i * fold_size);
        train_indices.insert(train_indices.end(), indices.begin() + (i + 1) * fold_size, indices.end());

        vector<vector<double>> X_train_fold;
        vector<int> y_train_fold;
        for (int idx : train_indices) {
            X_train_fold.push_back(X_train[idx]);
            y_train_fold.push_back(y_train[idx]);
        }

        vector<vector<double>> X_test_fold;
        vector<int> y_test_fold;
        for (int idx : test_indices) {
            X_test_fold.push_back(X_train[idx]);
            y_test_fold.push_back(y_train[idx]);
        }

        GaussianNaiveBayes model(best_lambda);
        model.fit(X_train_fold, y_train_fold);

        vector<int> y_pred_fold = model.predict(X_test_fold);
        double accuracy = accuracy_score(y_test_fold, y_pred_fold);
        fold_accuracies.push_back(accuracy);

        cout << "Fold " << i + 1 << ": Accuracy = " << accuracy * 100 << "%" << endl;
    }

    double mean_accuracy = accumulate(fold_accuracies.begin(), fold_accuracies.end(), 0.0) / k;
    double std_dev = sqrt(accumulate(fold_accuracies.begin(), fold_accuracies.end(), 0.0,
        [mean_accuracy](double sum, double acc) {
            return sum + (acc - mean_accuracy) * (acc - mean_accuracy);
        }) / k);

    cout << "\nMean accuracy over 10 folds: " << mean_accuracy * 100 << "%" << endl;
    cout << "Standard deviation: " << std_dev * 100 << "%" << endl;

    return 0;
}