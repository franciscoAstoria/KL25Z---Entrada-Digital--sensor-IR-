#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <pwm_z42.h>       

#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
#define SLEEP_TIME_MS 50

uint16_t duty_curva_frente  = TPM_MODULE*0.68; 
uint16_t duty_curva_tras  = TPM_MODULE*0.35; 


uint16_t duty_25  = TPM_MODULE*0.25; 
uint16_t duty_50  = TPM_MODULE*0.5;  
uint16_t duty_70  = TPM_MODULE*0.7;    
uint16_t duty_reta  = TPM_MODULE*0.75;  
uint16_t duty_off  = TPM_MODULE*0;  
uint16_t duty_on  = TPM_MODULE*1;  

#define SENSOR_NODE_ESQ DT_ALIAS(sensor_esq)
#define SENSOR_NODE_DIR DT_ALIAS(sensor_dir)

static const struct gpio_dt_spec sensor_esq =
    GPIO_DT_SPEC_GET(SENSOR_NODE_ESQ, gpios);

static const struct gpio_dt_spec sensor_dir =
    GPIO_DT_SPEC_GET(SENSOR_NODE_DIR, gpios);

int main(void)
{
    int config_sensor_esq, config_sensor_dir;

    pwm_tpm_Init(TPM1, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    pwm_tpm_Ch_Init(TPM1, 0, TPM_PWM_H, GPIOB, 0);
    pwm_tpm_Ch_Init(TPM1, 1, TPM_PWM_H, GPIOB, 1);

    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 2);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 3);

    // Verifica se a porta GPIO está pronta
    if (!device_is_ready(sensor_esq.port)) {
        printk("GPIO Sensor Esq não está pronto\n");
        return 0;
    }

    if (!device_is_ready(sensor_dir.port)) {
        printk("GPIO Sensor Dir não está pronto\n");
        return 0;
    }

    // Configura como entrada
    config_sensor_esq = gpio_pin_configure_dt(&sensor_esq, GPIO_INPUT);
    if (config_sensor_esq < 0) {
        printk("Erro ao configurar pino Sensor Esq\n");
        return 0;
    }

    config_sensor_dir = gpio_pin_configure_dt(&sensor_dir, GPIO_INPUT);
    if (config_sensor_dir < 0) {
        printk("Erro ao configurar pino Sensor Dir\n");
        return 0;
    }

    while (1) {
        int val_sensor_esq = gpio_pin_get_dt(&sensor_esq);
        int val_sensor_dir = gpio_pin_get_dt(&sensor_dir);

        if (val_sensor_esq == 1 && val_sensor_dir == 0){
            pwm_tpm_CnV(TPM1, 1, duty_curva_frente);
            pwm_tpm_CnV(TPM1, 0, duty_off);  
            pwm_tpm_CnV(TPM2, 0, duty_curva_tras);
            pwm_tpm_CnV(TPM2, 1, duty_off);       
            k_msleep(SLEEP_TIME_MS);  
            pwm_tpm_CnV(TPM1, 1, duty_off);
            pwm_tpm_CnV(TPM1, 0, duty_off);
            pwm_tpm_CnV(TPM2, 0, duty_off);
            pwm_tpm_CnV(TPM2, 1, duty_off);
        } else if (val_sensor_dir == 1 && val_sensor_esq == 0){
            pwm_tpm_CnV(TPM2, 1, duty_curva_frente);
            pwm_tpm_CnV(TPM2, 0, duty_off);   
            pwm_tpm_CnV(TPM1, 0, duty_curva_tras);
            pwm_tpm_CnV(TPM1, 1, duty_off);        
            k_msleep(SLEEP_TIME_MS);  
            pwm_tpm_CnV(TPM1, 1, duty_off);
            pwm_tpm_CnV(TPM1, 0, duty_off);
            pwm_tpm_CnV(TPM2, 0, duty_off);
            pwm_tpm_CnV(TPM2, 1, duty_off);
        }else{
            pwm_tpm_CnV(TPM1, 1, duty_reta);
            pwm_tpm_CnV(TPM1, 0, duty_off);  
            pwm_tpm_CnV(TPM2, 1, duty_reta);
            pwm_tpm_CnV(TPM2, 0, duty_off);        
            k_msleep(SLEEP_TIME_MS);
        }

    }
}