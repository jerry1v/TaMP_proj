#pragma once
#include <string>
#include <vector>

/**
 * @brief Hides a text message in the LSB of WAV audio samples.
 * @param wavData   Raw WAV file bytes.
 * @param message   Text message to hide.
 * @return Modified WAV bytes with message embedded, or empty on error.
 */
std::vector<char> stegoHide(const std::vector<char>& wavData,
                             const std::string& message);

/**
 * @brief Extracts a message previously hidden by stegoHide().
 * @param wavData   Raw WAV file bytes.
 * @return Extracted message, or empty string if not found.
 */
std::string stegoExtract(const std::vector<char>& wavData);
