// Package declaration matching project namespace
package com.example.lab22_nativecalculator;

// Android framework imports
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

// View binding import (auto-generated from layout file)
import com.example.lab22_nativecalculator.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Native method declarations - implemented in native-lib.cpp
    public native String fetchEngineStatus();
    public native int exponentiate(int base_value, int power_value);
    public native String processText(String input_text);
    public native int calculateAverage(int[] data_array);

    // View binding instance
    private ActivityMainBinding binding;

    // Static initializer block - loads native library at class load time
    static {
        System.loadLibrary("lab22_nativecalculator");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Initialize view binding
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Execute native function demonstrations
        demonstrateNativeCalls();
    }

    private void demonstrateNativeCalls() {
        // Test 1: Retrieve engine initialization message
        String engine_status = fetchEngineStatus();
        binding.statusDisplay.setText(engine_status);

        // Test 2: Perform power calculation
        int base_input = 4;
        int exponent_input = 6;
        int power_result = exponentiate(base_input, exponent_input);

        if (power_result >= 0) {
            binding.powerDisplay.setText("4^6 = " + power_result);
        } else {
            binding.powerDisplay.setText("Computation failed (code: " + power_result + ")");
        }

        // Test 3: Transform text string
        String sample_phrase = "Cross-Platform Computing";
        String transformed_phrase = processText(sample_phrase);
        binding.stringDisplay.setText("Original: " + sample_phrase +
                "\nReversed: " + transformed_phrase);

        // Test 4: Calculate array average
        int[] number_set = {85, 92, 78, 96, 88, 73, 90};
        int average_result = calculateAverage(number_set);
        binding.arrayDisplay.setText("Array Average: " + average_result);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Clean up binding reference to prevent memory leaks
        binding = null;
    }
}