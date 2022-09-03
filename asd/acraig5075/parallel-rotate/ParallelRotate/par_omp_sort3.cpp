void grid_to_file(int it)
{
    stringstream fname;
    fstream f1;
    fname << "output" << "_" << it << ".dat";
    f1.open(fname.str().c_str(), ios_base::out);
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 50; j++){
            f1 << grid[i][j] << "\t";
		}
        f1 << endl;
    }
    f1.close();
}
