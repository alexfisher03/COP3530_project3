import React from 'react'
import Box from '@mui/material/Box';
import Checkbox from '@mui/material/Checkbox';
import FormControlLabel from '@mui/material/FormControlLabel';
import TextField from '@mui/material/TextField';
import Autocomplete from '@mui/material/Autocomplete';

// TODO: Defines the filter checkbox component
export default function IndeterminateCheckbox() {
    const [checked, setChecked] = React.useState([true, false]);
  
    const handleChange1 = (event) => {
      setChecked([event.target.checked, event.target.checked]);
    };
  
    const handleChange2 = (event) => {
      setChecked([event.target.checked, checked[1]]);
    };
  
    const handleChange3 = (event) => {
      setChecked([checked[0], event.target.checked]);
    };

    const children = (
      <Box sx={{ display: 'flex', flexDirection: 'column', ml: 3 }}>
        <FormControlLabel
          label="Car Model 1"
          control={<Checkbox checked={checked[0]} onChange={handleChange2} />}
        />
        <FormControlLabel
          label="Car Model 2"
          control={<Checkbox checked={checked[1]} onChange={handleChange3} />}
        />
      </Box>
    );

    const carManufacturers = [
        { label: "BMW" },
        { label: "Mercedes" },
        { label: "Audi" },
        { label: "Volkswagen" },
        { label: "Porsche" },
        { label: "Nissan" },
        { label: "Toyota" },
        { label: "Honda" },
        { label: "Hyundai" },
        { label: "Ferrari" },
        { label: "Jaguar" },
    ];
  
    return (
      <>
        <div className='m-3'>Filter by Car Manufacturer:</div>

        <div className='m-3'>
            <Autocomplete
                disablePortal
                id="combo-box-demo"
                options={carManufacturers}
                sx={{ width: 300 }}
                renderInput={(params) => <TextField {...params} label="Search for Manufacturer" />}
            />
        </div>
        
        <div className='m-3'>
            <FormControlLabel
            label="Car Brand"
            control={
                <Checkbox
                checked={checked[0] && checked[1]}
                indeterminate={checked[0] !== checked[1]}
                onChange={handleChange1}
                />
            }
            />
            {children}
        </div>
        
      </>
    );
  } 
